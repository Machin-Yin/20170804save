#include "imgwindow.h"
#include <QOpenGLContext>
#include <QGuiApplication>

ImgWindow::ImgWindow(QScreen *screen)
    : QWindow(screen)
{
    setSurfaceType(QSurface::OpenGLSurface);
    QWindow::resize(960,192);

    QSurfaceFormat format;
    if( QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL ){

    }
}

ImgWindow::~ImgWindow()
{

}
