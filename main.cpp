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
    nabludatel(QString& path) { file(path); } //исправить

public slots:
    void checkState(QFileInfo&);

signals:
    void stateChanged();

private:
    QFileInfo file;
};

void nabludatel::checkState(QFileInfo& file) {
    QFileInfo file2 = file;
    file.refresh();
    if (file != file2) {
        emit stateChanged();
    }
}

void nabludatel::stateChanged() {
    cout << "File changed" << Qt::endl;
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
}
