#ifndef CONSOLELOGGER_H
#define CONSOLELOGGER_H

#include "ilogger.h"
#include "pch.h"

class ConsoleLogger : public ILogger {
public:
    void log(const QString& message) override {
        qDebug() << message;
    }
};

#endif // CONSOLELOGGER_H
