#include <QApplication>
#include <QFile>
#include <QStyleFactory>
#include "include/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyle(QStyleFactory::create("Fusion"));

    // Load QSS from file
    QFile styleFile(":/resources/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString style = QLatin1String(styleFile.readAll());
        a.setStyleSheet(style);
    }

    MainWindow w;
    w.show();
    return a.exec();
}
