#ifndef SHOWMOREH
#define SHOWMOREH

#include <QWidget>
#include "classwidget.h"
#include "classtop.h"
#include "jsonfunc.h"

class ShowMore : public QWidget
{
    Q_OBJECT

public:
    explicit ShowMore(QWidget *parent = 0);
    QWidget *moreWidget;
    void setElementName(int category,const CLASSSTRUCTMAP &classStruct);
    void setTopName(int category,const CATEGORYMAP &cateGoryMap);
    void setElementNum(const ELEMENTNUMBERMAP &elementNum);
    void setElementImage(int category,const CLASSSTRUCTMAP &classStructMap);

private:
    ClassTop *moreClassTop;
    QVBoxLayout *mainLayout;
    Element *moreElement;
    QGridLayout *eleLayout;
    QWidget  *spaceWidget;

    bool eventFilter(QObject *watched, QEvent *event);
    int categoryFlag;
    int elementNumber;
    int starIsOK;
};

#endif // SHOWMOREH
