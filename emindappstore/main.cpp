#include "softwarecenterwindow.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SoftwareCenterWindow w;
    SoftwareCenterWindow::setWinStyle(":/style.qss");
    w.show();

    return a.exec();
}
