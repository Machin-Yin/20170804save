#ifndef SOFTWARECENTERWINDOWH
#define SOFTWARECENTERWINDOWH


#include <QWidget>
#include <QFile>
#include <QApplication>
#include <QDebug>
#include "jsonfunc.h"
#include "sharedata.h"

class TitleBar;
class QStackedWidget;
class QHBoxLayout;
class QVBoxLayout;
class HomeWidget;
class SearchWidget;
class DetailWidget;
class QScrollArea;
class ClassPage;
class UpdatePage;
class ManagerPage;


class SoftwareCenterWindow : public QWidget
{
    Q_OBJECT

public:
    SoftwareCenterWindow(QWidget *parent = 0);
    ~SoftwareCenterWindow();

    static void setWinStyle(const QString& qssFile){
        QFile qss(qssFile);
        if(qss.open(QFile::ReadOnly)){
        qApp->setStyleSheet(qss.readAll());
        qss.close();
        }else{
            qDebug()<<__FUNCTION__<<" QSS file Open Failed! "<<endl;
        }
    }

signals:
    void toSearch(QString);
    void pageChanged();
    void refreshPg(int);

protected slots:
    void onBtnHome();
    void onBtnClass();
    void onBtnMore();
    void onBtnUpdate();
    void onBtnManager();
    void mousePressEvent(QMouseEvent* event);
    void toPageSearch();
    void toPageDetail();
    void nextPage();
    void backPage();
    void refreshPage();
    void onPageChanged(int index);

private:

    QStackedWidget *stackWidget;
    TitleBar *titleBar;
    QVBoxLayout *vly;

    HomeWidget *pageHome;
    ClassPage *pageClass;
    UpdatePage *pageUpdate;
    ManagerPage *pageManager;
    SearchWidget *pageSearch;
    DetailWidget *pageDetail;

    Qt::WindowFlags flags;
    QList<int> pageList;
    QScrollArea *scrollArea;
    qint32 pageIndex;
    JSONFUNC *jsonFunc;
    ShareData *shareData;
};

#endif // SoftwareCenterWindowH
