#include "Nabludatel.h"

Nabludatel::Nabludatel(QObject *parent) : QObject(parent){}

void Nabludatel::setFiles(const QString &path)
{
    if (!QFile::exists(path)) {
        qDebug() << "This file does not exist!";
        return;
    }

    QFile sourceFile(path);
    QVector<QFileInfo> newVector;

    if (sourceFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&sourceFile);
        while (!in.atEnd()) {
            QString path = in.readLine();

            if (path.trimmed().isEmpty()) {
                continue;
            }

            QFileInfo file(path);

            if (!file.isFile()) {
                continue;
            }

            newVector.append(file);
        }
        sourceFile.close();
    } else {
        qDebug() << "Error opening source file" << Qt::endl;
        return;
    }

    findChanges(newVector);

    files = newVector;
}

void Nabludatel::checkFilesState()
{
    for (QFileInfo& currentFile : files) {
        QFileInfo oldFile = currentFile;
        currentFile.refresh();

        if (currentFile.exists() != oldFile.exists() ||
            currentFile.size() != oldFile.size()) {

            emit stateChanged();
        }
    }
}

void Nabludatel::findChanges(const QVector<QFileInfo> &newFiles)
{
    QSet<QString> oldPaths;
    for (const QFileInfo& file: files) {
        oldPaths.insert(file.absoluteFilePath());
    }

    QSet<QString> newPaths;
    for (const QFileInfo& file : newFiles) {
        newPaths.insert(file.absoluteFilePath());
    }

    QSet<QString> removedFiles = oldPaths;
    removedFiles.subtract(newPaths);

    for (const QString& path : removedFiles) {
        qDebug() << "File removed from watch: " << path << Qt::endl;
    }

    QSet<QString> addedFiles = newPaths;
    addedFiles.subtract(oldPaths);

    for (const QString& path : addedFiles) {
        qDebug() << "Added file on watch: " << path << Qt::endl;
        QFileInfo file(path);

        if (!file.exists()) {
            qDebug() << "   Status: file does not exist" << Qt::endl;
        } else {
            qDebug() << "   Status: file exist, size " << file.size() << Qt::endl;
        }
    }
}
