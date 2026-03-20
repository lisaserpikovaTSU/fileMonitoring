#ifndef NABLUDATEL_H
#define NABLUDATEL_H

#include "qfileinfo.h"
#include <QObject>

class Nabludatel: public QObject
{
    Q_OBJECT
public:
    explicit Nabludatel(QObject *parent = nullptr);

    void setFile(const QString &path);

signals:
    void stateChanged();

public slots:
    void checkFileState();

private:
    QFileInfo file;
};

#endif // NABLUDATEL_H
