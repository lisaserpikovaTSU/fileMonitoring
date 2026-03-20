#include "Nabludatel.h"
#include <QCoreApplication>
#include <QFileInfo>
#include <QString>
#include <QTextStream>
#include <QTimer>
#include <thread>
#include <chrono>
#include <QObject>

QTextStream cin(stdin);
QTextStream cout(stdout);

void printMes()
{
    cout << "State of file was changed" << Qt::endl;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QString path;
    cout << "Input file path: " << Qt::endl;
    path = cin.readLine();

    Nabludatel f;
    f.setFile(path);

    QObject::connect(&f, &Nabludatel::stateChanged, printMes);

    while (true) {
        f.checkFileState();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
    // для проверки: /Users/liza/Desktop/hint.png
    // C:\Users\st22.297\Downloads\qqq.txt
}
