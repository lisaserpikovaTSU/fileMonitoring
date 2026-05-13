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

void Nabludatel::checkFileState()
{
    QFileInfo oldfile = file;
    file.refresh();
    if (file.exists() != oldfile.exists() ||
        file.size() != oldfile.size()) {

        emit stateChanged();
    }
}
