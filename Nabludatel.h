#ifndef NABLUDATEL_H
#define NABLUDATEL_H

#include "pch.h"

class Nabludatel: public QObject
{
    Q_OBJECT
public:
    explicit Nabludatel(QObject *parent = nullptr);

    void setFiles(const QVector<QFileInfo> newFiles);

signals:
    void fileCreated(const QString& path);
    void fileDeleted(const QString& path);
    void sizeChanged(const QString& path, int oldSize, int newSize);

public slots:
    void checkFilesState();

private:
    QVector<QFileInfo> files;
};

#endif // NABLUDATEL_H
