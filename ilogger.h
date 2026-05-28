#ifndef ILOGGER_H
#define ILOGGER_H

#include <QString>

//Интерфейс для логгирования
class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(const QString& message) = 0;
};

#endif // ILOGGER_H
