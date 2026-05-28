#ifndef CONSOLELOGGER_H
#define CONSOLELOGGER_H

#include "ilogger.h"
#include "pch.h"

//Реализация логгера для вывода в консоль
//Выводит сообщения через qDebug()
class ConsoleLogger : public ILogger {
public:
    void log(const QString& message) override {
        qDebug() << message;
    }
};

#endif // CONSOLELOGGER_H
