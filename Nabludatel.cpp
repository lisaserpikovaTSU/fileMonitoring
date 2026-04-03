#include "Nabludatel.h"

Nabludatel::Nabludatel(QObject *parent) : QObject(parent){}

void Nabludatel::setFile(const QString &path)
{
    file = QFileInfo(path);
    //print initial state
    if (file.exists()) {
        qDebug() << "File exists. Size: " << file.size() << Qt::endl;
    }
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
