#include "softwarecenterwindow.h"
#include <QApplication>
#include <QIcon>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator *qTranslator = new QTranslator();
    qTranslator->load("app_zh.qm");
    a.installTranslator(qTranslator);
    SoftwareCenterWindow w;
    SoftwareCenterWindow::setWinStyle(":/style.qss");
    w.show();

    return a.exec();
}
