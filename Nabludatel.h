#ifndef NABLUDATEL_H
#define NABLUDATEL_H

#include "pch.h"

//Класс для наблюдения за состоянием файлов
//Отслеживает изменения файлов:
// - Создание нового файла
// - Удаление файла
// - Изменение размера файла

//Использует Qt-сигналы для уведомления об изменениях
class Nabludatel: public QObject
{
    Q_OBJECT

public:
    //Конструктор, parent - Родительский Qt-объект
    explicit Nabludatel(QObject *parent = nullptr);

    //Установить список файлов для наблюдения
    //newFiles - вектор с информацией о файлах для отслеживания
    void setFiles(const QVector<QFileInfo> newFiles);

    //Проверить состояние всех наблюдаемых файлов
    //сравнивает текущее состояние с предыдущим,
    //при обнаружении изменений генерирует соответствующие сигналы
    void checkFilesState();

signals:
    //файл был создан
    void fileCreated(const QString& path);

    //файл был удален
    void fileDeleted(const QString& path);

    //размер файла изменился
    void sizeChanged(const QString& path, int oldSize, int newSize);

private:
    QVector<QFileInfo> files;  // Вектор наблюдаемых файлов
};

#endif // NABLUDATEL_H
