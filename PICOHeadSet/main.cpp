#include <QApplication>
#include <windows.h>
#include <QDebug>
#include <QTimer>
#include "src/ui/VideoLayoutManager/mainwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QIcon icon(":/app.ico");
    a.setWindowIcon(icon);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    MainWidget w;
    w.show();

    return a.exec();
}
