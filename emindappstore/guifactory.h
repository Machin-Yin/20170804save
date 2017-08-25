#ifndef GUIFACTORY_H
#define GUIFACTORY_H

#include <QObject>
#include <qglobal.h>

class GuiFactory : public QObject
{
    Q_OBJECT
public:
    explicit GuiFactory(QObject *parent = 0);

signals:

public slots:
};

#endif // GUIFACTORY_H