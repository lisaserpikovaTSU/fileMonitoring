#include <QCoreApplication>
#include <QFileInfo>
#include <QString>
#include <QTextStream>
#include <QTimer>
#include <thread>
#include <chrono>

QTextStream cin(stdin);
QTextStream cout(stdout);

void CheckFileState(QFileInfo& file) {
    file.refresh();
    if (file.exists()) {
        cout << "File exist, size: " << file.size() << Qt::endl;
    } else {
        cout << "File not exist" << Qt::endl;
    }
}

int main()
{

    QString path;
    cout << "Input file path: " << Qt::endl;
    path = cin.readLine();

    QFileInfo file(path);

    if (!file.isFile()) {
        cout << "Incorrect path" << Qt::endl;
        return 1;
    }

    while (true) {
        CheckFileState(file);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
    // для проверки: /Users/liza/Desktop/hint.png
}
