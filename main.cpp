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

//Проверка формата файла (должен быть .txt)
bool isValidFormat(const QString& path) {
    QFileInfo fileInfo(path);
    QString suffix = fileInfo.suffix().toLower();
    return suffix == "txt";
}

//Чтение путей из файла-источника
QVector<QFileInfo> getPaths(const QString& path) {
    //Проверка существования файла-источника
    if (!QFile::exists(path)) {
        qDebug() << "This file does not exist!";
        return QVector<QFileInfo>();
    }

    QFile sourceFile(path);
    QVector<QFileInfo> newVector;

    if (sourceFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&sourceFile);

        while (!in.atEnd()) {
            QString linePath = in.readLine();

            //Пропускаем пустые строки
            if (linePath.trimmed().isEmpty()) {
                continue;
            }

            QFileInfo file(linePath);

            //Пропускаем директории
            if (file.isDir()) {
                qDebug() << "Skipping directory (only files are supported)!" << Qt::endl;
                continue;
            }

            //Добавляем файл в список наблюдения
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

    //Запрос пути к файлу-источнику
    qDebug() << "Enter path to source file: ";
    QString source = cin.readLine();

    //Проверка существует ли файл
    if (!QFile::exists(source)) {
        qDebug() << "ERROR: Source file does not exist!";
        return 1;
    }

    //Проверка формата (.txt)
    if (!isValidFormat(source)) {
        qDebug() << "ERROR: Invalid file format. Only .txt files are supported!";
        return 1;
    }

    //Создание компонентов
    ConsoleLogger logger;      //Логгер для вывода сообщений
    SecondsTimer timer;        //Таймер для задержек

    Nabludatel nabludatel;     //Наблюдатель за файлами
    nabludatel.setFiles(getPaths(source));

    //ПОДКЛЮЧЕНИЕ СИГНАЛОВ К ЛОГГЕРУ
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

    //Информация для отслеживания изменений файла-источника
    QFileInfo sourceOfPaths(source);
    QFileInfo oldSourceOfPaths(source);

    //ОСНОВНОЙ ЦИКЛ МОНИТОРИНГА
    while (true) {
        //Проверяем, не изменился ли файл-источник
        sourceOfPaths.refresh();
        if (sourceOfPaths.lastModified() != oldSourceOfPaths.lastModified()){
            //Файл-источник изменился - перезагружаем список файлов
            QVector<QFileInfo> newPaths = getPaths(source);
            nabludatel.setFiles(newPaths);
            oldSourceOfPaths = sourceOfPaths;
        }

        //Проверяем состояние всех наблюдаемых файлов
        nabludatel.checkFilesState();

        //Пауза перед следующим циклом (2 секунды)
        timer.sleepFor(2);
    }

    return 0;
}
