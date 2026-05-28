#include "Nabludatel.h"

//Реализация наблюдателя
Nabludatel::Nabludatel(QObject *parent) : QObject(parent) {}

//Установка нового списка файлов для наблюдения
void Nabludatel::setFiles(const QVector<QFileInfo> newFiles)
{
    //Собираем пути из старого списка файлов
    QSet<QString> oldPaths;
    for (const QFileInfo& file: files) {
        oldPaths.insert(file.absoluteFilePath());
    }

    //Собираем пути из нового списка файлов
    QSet<QString> newPaths;
    for (const QFileInfo& file : newFiles) {
        newPaths.insert(file.absoluteFilePath());
    }

    //Находим файлы, которые были удалены из списка наблюдения
    QSet<QString> removedFiles = oldPaths;
    removedFiles.subtract(newPaths);

    for (const QString& path : removedFiles) {
        qDebug() << "File removed from watch: " << path << Qt::endl;
    }

    //Находим новые файлы для наблюдения
    QSet<QString> addedFiles = newPaths;
    addedFiles.subtract(oldPaths);

    for (const QString& path : addedFiles) {
        qDebug() << "Added file on watch: " << path;
        QFileInfo file(path);

        //Выводим текущий статус файла
        if (!file.exists()) {
            qDebug() << "   Status: file does not exist" << Qt::endl;
        } else {
            qDebug() << "   Status: file exist, size " << file.size() << Qt::endl;
        }
    }

    //Сохраняем новый список файлов
    files = newFiles;
}

//Проверка состояния всех наблюдаемых файлов
void Nabludatel::checkFilesState()
{
    for (QFileInfo& currentFile : files) {
        QFileInfo oldFile = currentFile;  //Сохраняем предыдущее состояние
        currentFile.refresh();             //Обновляем информацию о файле

        //СЛУЧАЙ 1: Файл появился
        if (!oldFile.exists() && currentFile.exists()) {
            emit fileCreated(currentFile.absoluteFilePath());
        }
        //СЛУЧАЙ 2: Файл исчез
        else if (oldFile.exists() && !currentFile.exists()) {
            emit fileDeleted(currentFile.absoluteFilePath());
        }
        //СЛУЧАЙ 3: Размер файла изменился
        else if (currentFile.size() != oldFile.size()) {
            emit sizeChanged(currentFile.absoluteFilePath(),
                             oldFile.size(),
                             currentFile.size());
        }
    }
}
