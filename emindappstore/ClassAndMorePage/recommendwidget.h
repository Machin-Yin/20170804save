#ifndef RECOMMENDWIDGET_H
#define RECOMMENDWIDGET_H

#include <QWidget>
#include "classwidget.h"
#include "classtop.h"

class RecommendWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RecommendWidget(QWidget *parent = 0);
    void setElementName(const CLASSSTRUCTMAP &classStruct);
    void setTopName();
    void setElementImage(const CLASSSTRUCTMAP &classStructMap);

private:
    ClassTop *classTop;
    QVBoxLayout *mainLayout;
    Element *element;
    QGridLayout *eleLayout;
    QWidget  *spaceWidget;
    JSONFUNC *jsonFunc;
    QSpacerItem *spacer;

    bool eventFilter(QObject *watched, QEvent *event);
    int categoryFlag;

signals:

public slots:
};

#endif // RECOMMENDWIDGET_H
