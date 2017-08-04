#include "classpage.h"

ClassPage::ClassPage(QWidget *parent) : QWidget(parent)
{
    scrollClass = new QScrollArea(this);
    shareData = new ShareData();
    jsonFunc = new JSONFUNC(shareData);
    jsonFunc->getCategoryNum();
    moreClassWidget = new MorePage();
    scrollClass->resize(QSize(960,640));
//    scrollClass->setMaximumWidth(1200);
    pageClassWidget = new QWidget();

    connect(jsonFunc,SIGNAL(productIsOk()),moreClassWidget,SLOT(createMorewindow()),Qt::QueuedConnection);
    connect(jsonFunc,SIGNAL(categoryIsOk(int)),this,SLOT(createClassWindow(int)),Qt::QueuedConnection);
    connect(jsonFunc,SIGNAL(productIsOk()),this,SLOT(setClassElementName()),Qt::QueuedConnection);
//    connect(jsonFunc,SIGNAL(updateIsOk()),this,SLOT(testUpdateMap()));
}

bool ClassPage::event(QEvent *event)
{
    if(event->type() == QEvent::Resize)
    {
        scrollClass->resize(QSize(this->width(),this->height()));
        return true;
    }
    return QWidget::event(event);
}

//创建分类页
void ClassPage::createClassWindow(int catenum)
{
    cateNum = catenum;
    classWidget = new ClassWidget[catenum];
    vbClasslayout = new QVBoxLayout();
//    pageClassWidget = new QWidget();

    vbClasslayout = new QVBoxLayout();
    scrollClass->setFrameShape(QFrame::NoFrame); //去除窗口边框

    for(int i=0;i<catenum;i++)
    {
        connect(&classWidget[i],SIGNAL(moreShow(int)),this,SLOT(setMoreShow(int)));
        classWidget[i].setCategory(i);
        classWidget[i].setTopName(shareData->categoryMap);
        vbClasslayout->addWidget(classWidget[i].widget);
    }

    pageClassSpacer =new QSpacerItem(24,24,QSizePolicy::Minimum,QSizePolicy::Expanding);
    scrollClass->setWidget(pageClassWidget);
    vbClasslayout->addSpacerItem(pageClassSpacer);
    vbClasslayout->setMargin(0);
    pageClassWidget->setLayout(vbClasslayout);
    //滚动条不可见，只能通过鼠标滑动
    scrollClass->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollClass->setWidgetResizable(true);
    jsonFunc->setAppname();
}


//设置分类软件的属性
void ClassPage::setClassElementName()
{
    qDebug()<<__FUNCTION__<<endl;
    if(shareData->classStrMap.isEmpty())
    {
        qDebug()<<"classStrMap is Empty!"<<endl;
    }

    for(int i = 0;i<cateNum;i++)
    {
        classWidget[i].initElement(shareData->classElementNumMap);
        classWidget[i].setElementName(shareData->classStrMap);
        classWidget[i].setElementImage(shareData->classStrMap);
    }

    int a[5] = {1,2,33,44,55};
    jsonFunc->getUpdateRelease(a,5);
}

//测试更多页面跳转
void ClassPage::setMoreShow(int i)
{
    moreClassWidget->showMore->setTopName(i,shareData->categoryMap);
    moreClassWidget->showMore->setElementNum(shareData->classElementNumMap);
    moreClassWidget->showMore->setElementName(i,shareData->classStrMap);
    moreClassWidget->showMore->setElementImage(i,shareData->classStrMap);
    moreClassWidget->scrollMore->resize(this->size().width(),this->size().height());
    emit setMore();
}

void ClassPage::testUpdateMap()
{
    if(!shareData->updateStrMap.isEmpty())
    {
        UPDATESTRUCTMAP::iterator it = shareData->updateStrMap.begin();

        for(;it!=shareData->updateStrMap.end();++it)
        {
            qDebug()<<"release id = "<<it.key()<<"  version = "<<it.value().version<<"  packsize = "<<it.value().packageSize;
        }
    }
}
