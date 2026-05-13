#ifndef NABLUDATEL_H
#define NABLUDATEL_H

#include "pch.h"

class Nabludatel: public QObject
{
    Q_OBJECT
public:
    explicit Nabludatel(QObject *parent = nullptr);

    void setFiles(const QString &path);

signals:
    void stateChanged();

public slots:
    void checkFileState();

private:
    QVector<QFileInfo> files;

    void findChanges(const QVector<QFileInfo>& newFiles);
};

#endif // NABLUDATEL_H
