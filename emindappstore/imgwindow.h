#ifndef IMGWINDOW_H
#define IMGWINDOW_H

#include <qglobal.h>
#include <QWindow>
#include <QtWidgets>
#include <QObject>

class ImgWindow : public QWindow
{
    Q_OBJECT
public:
    explicit ImgWindow(QScreen* screen=0);
    ~ImgWindow();
};

#endif // IMGWINDOW_H
