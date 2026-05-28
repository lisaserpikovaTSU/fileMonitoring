#include "pch.h"
#include "Nabludatel.h"
#include "consolelogger.h"
#include "secondstimer.h"
#include <memory>
#include <QDateTime>

//Тестирование случаев для файла-источника

//Входные данные - путь до txt файла

//Случай 1: файл не txt формата
//   Путь для проверки: ../tests/source.pdf

//Случай 2: файла по переданному пути не существует
//   Путь для проверки: ../tests/sourc.txt

//Случай 3: файл пустой
//   Путь для проверки: ../tests/source_empty.txt

//Случай 4: файл содержит пути до директорий
//   Путь для проверки: ../tests/source_folders.txt

//Тестирование случаев для состояний файлов

//Случай 5: Файла не было - файл был создан
//   Путь для проверки: ../tests/source.txt

//Случай 6: Файл был - файл удалили
//   Путь для проверки: ../tests/source.txt

//Случай 7: Файл был - файл поменял размер
//   Путь для проверки: ../tests/source.txt


QTextStream cin(stdin);

bool isValidFormat(const QString& path) {
    QFileInfo fileInfo(path);
    QString suffix = fileInfo.suffix().toLower();
    return suffix == "txt";
}

QVector<QFileInfo> getPaths(const QString& path) {
    if (!QFile::exists(path)) {
        qDebug() << "This file does not exist!";
        return QVector<QFileInfo>(); //Возвращаем пустой вектор
    }

    QFile sourceFile(path);
    QVector<QFileInfo> newVector;

    if (sourceFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&sourceFile);
        while (!in.atEnd()) {
            QString linePath = in.readLine();

            if (linePath.trimmed().isEmpty()) {
                continue;
            }

            QFileInfo file(linePath);

            if (file.isDir()) {
                qDebug() << "Skipping directory (only files are supported)!" << Qt::endl;
                continue;
            }

            newVector.append(file);
        }
        sourceFile.close();
    } else {
        qDebug() << "Error opening source file" << Qt::endl;
        return QVector<QFileInfo>();
    }

    return newVector;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qDebug() << "Enter path to source file: ";
    QString source = cin.readLine();

    if (!QFile::exists(source)) {
        qDebug() << "ERROR: Source file does not exist!";
        return 1;
    }

    if (!isValidFormat(source)) {
        qDebug() << "ERROR: Invalid file format. Only .txt files are supported!";
        return 1;
    }

    //Создаем логгер и таймер
    ConsoleLogger logger;
    SecondsTimer timer;

    Nabludatel nabludatel;
    nabludatel.setFiles(getPaths(source));

    //Cоединение сигналов с лямбда-функциями, использующими логгер
    QObject::connect(&nabludatel, &Nabludatel::fileCreated,
                     [&logger](const QString& path) {
                         logger.log("File created: " + path);
                     });

    QObject::connect(&nabludatel, &Nabludatel::fileDeleted,
                     [&logger](const QString& path) {
                         logger.log("File deleted: " + path);
                     });

    QObject::connect(&nabludatel, &Nabludatel::sizeChanged,
                     [&logger](const QString& path, int oldSize, int newSize) {
                         logger.log(QString("Size changed: %1 (%2 -> %3 bytes)")
                                        .arg(path).arg(oldSize).arg(newSize));
                     });

    QFileInfo sourceOfPaths(source);
    QFileInfo oldSourceOfPaths(source);

    while (true) {
        sourceOfPaths.refresh();
        if (sourceOfPaths.lastModified() != oldSourceOfPaths.lastModified()){
            QVector<QFileInfo> newPaths = getPaths(source);
            nabludatel.setFiles(newPaths);
            oldSourceOfPaths = sourceOfPaths;
        }

        nabludatel.checkFilesState();
        timer.sleepFor(2); //Используем таймер
    }

    return 0;
}

// /Users/liza/fileMonitoring/tests/source.txt
