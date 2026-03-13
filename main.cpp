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

class nabludatel : public QObject
{
    Q_OBJECT

public:
    explicit nabludatel(const QString& path, QObject *parent = nullptr): QObject(parent), file(path) {}

public slots:
    void checkState();

signals:
    void stateChanged();

private:
    QFileInfo file;
};

void nabludatel::checkState() {
    static QFileInfo oldfile = file;
    file.refresh();
    if (file.exists() != oldfile.exists() ||
        file.size() != oldfile.size()) {

        emit stateChanged();
        oldfile = file;
        cout << "State of file was changed" << Qt::endl;
    }
}

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

    nabludatel f(path);

    QObject::connect(&f, &nabludatel::stateChanged, [&](){
        cout << "signal received" << Qt::endl;
        cout.flush();
    });

    while (true) {
        f.checkState();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    /*
    QFileInfo file(path);

    if (file.isDir()) {
        cout << "Incorrect path, you've inputed a directory" << Qt::endl;
        return 1;
    }

    while (true) {
        CheckFileState(file);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    */

    return 0;
    // для проверки: /Users/liza/Desktop/hint.png
    // C:\Users\st22.297\Downloads\qqq.txt
}
