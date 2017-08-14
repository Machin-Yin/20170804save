#ifndef MOREPAGE_H
#define MOREPAGE_H

#include <QWidget>
#include "showmore.h"

class MorePage: public QWidget
{
    Q_OBJECT

public:
    MorePage(QWidget *parent = 0);
    QScrollArea  *scrollMore;
    ShowMore *showMore;

private:
    QSpacerItem *pageMoreSpacer;
    QVBoxLayout *vbClasslayoutMore;
    QWidget *pageMoreWidget;
    bool event(QEvent *event);


signals:
    void installpackage(QString,int);
    void updatePackage(QString,int);

protected slots:
    void sendInstallPackage(QString name ,int id);
    void sendUpdatePackage(QString name ,int id);
    void createMorewindow();
};

#endif // MOREPAGE_H
