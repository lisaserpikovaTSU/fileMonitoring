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
    void stateChanged();

public slots:
    void checkFilesState();

private:
    QVector<QFileInfo> files;
};

#endif // NABLUDATEL_H
