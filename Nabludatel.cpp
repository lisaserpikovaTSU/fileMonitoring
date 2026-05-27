#include "Nabludatel.h"

Nabludatel::Nabludatel(QObject *parent) : QObject(parent){}

void Nabludatel::setFiles(const QVector<QFileInfo> newFiles)
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

    files = newFiles;
}

void Nabludatel::checkFilesState()
{
    for (QFileInfo& currentFile : files) {
        QFileInfo oldFile = currentFile;
        currentFile.refresh();
        if (!oldFile.exists() && currentFile.exists()) {
            emit fileCreated(currentFile.absolutePath());
        }
        else if (oldFile.exists() && !currentFile.exists()) {
            emit fileDeleted(currentFile.absolutePath());
        }
        else if (currentFile.size() != oldFile.size()) {
            emit sizeChanged(currentFile.absolutePath());
        }
    }
}
