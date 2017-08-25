#ifndef SearchWidgetH
#define SearchWidgetH

#include <QWidget>
#include "ClassAndMorePage/classwidget.h"
#include "ClassAndMorePage/classtop.h"
#include "jsonfunc.h"
#include "sharedata.h"
#include <QVBoxLayout>
class QSPacerItem;

class SearchWidget : public QWidget
{
    Q_OBJECT

public:
    SearchWidget(QWidget *parent,ShareData* shareData,JSONFUNC *jsonFunc);
    ~SearchWidget();
    QWidget *moreWidget;
    void setElement(int category,const CLASSSTRUCTMAP &classStruct);
    void setElement(QString keyText1,const CLASSSTRUCTMAP &classStruct);
    void setTopName(QString keyText);
    void setElementNum(const ELEMENTNUMBERMAP &elementNum);

private:
    ClassTop *moreClassTop;
    QVBoxLayout *mainLayout;
    Element *moreElement;
    QGridLayout *eleLayout;
    QWidget  **spaceWidget;

    bool eventFilter(QObject *watched, QEvent *event);
    int categoryFlag;
    int elementNumber;
    ShareData *sdata;
    JSONFUNC *jdata;
    QVBoxLayout *vly;
    QSpacerItem *spItem;

signals:
    void installApp(QString,int);
    void updateApp(QString,int);

protected slots:
    void sendInstallApp(QString name,int id);
    void sendUpdateApp(QString name,int id);
    void updatePackageStatus(QString name, bool bo, int flag);

    void doSearch(QString keyText);
    void refreshPage(int);
};

#endif // SearchWidgetH
