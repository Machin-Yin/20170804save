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

protected slots:
    void createMorewindow();
};

#endif // MOREPAGE_H
