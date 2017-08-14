#include "classpage.h"


ClassPage::ClassPage(QWidget *parent, JSONFUNC *json, ShareData *data) : QWidget(parent)
{
    scrollClass = new QScrollArea(this);
    shareData = data;
    jsonFunc = json;
    jsonFunc->getCategoryNum();
    moreClassWidget = new MorePage();
    scrollClass->resize(QSize(960,640));
    scrollClass->setMaximumWidth(1200);
    pageClassWidget = new QWidget();

    connect(jsonFunc,SIGNAL(productIsOk()),moreClassWidget,SLOT(createMorewindow()),Qt::QueuedConnection);
    connect(jsonFunc,SIGNAL(categoryIsOk(int)),this,SLOT(createClassWindow(int)),Qt::QueuedConnection);
    connect(jsonFunc,SIGNAL(productIsOk()),this,SLOT(setClassElement()),Qt::QueuedConnection);
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

void ClassPage::resetStatus()
{
    for(int i = 0;i<cateNum;i++)
    {
        classWidget[i].resetStatus(shareData->classStrMap);
    }
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
        connect(&classWidget[i],SIGNAL(installApp(QString,int)),this,SLOT(sendInstallPackage(QString,int)));
        connect(&classWidget[i],SIGNAL(updateApp(QString,int)),this,SLOT(sendUpdatePackage(QString,int)));

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
void ClassPage::setClassElement()
{
    qDebug()<<__FUNCTION__<<endl;
    if(shareData->classStrMap.isEmpty())
    {
        qDebug()<<"classStrMap is Empty!"<<endl;
    }

    for(int i = 0;i<cateNum;i++)
    {
        classWidget[i].initElement(shareData->classElementNumMap);
        classWidget[i].setElement(shareData->classStrMap);
    }
    jsonFunc->getRecommend();
}

//测试更多页面跳转
void ClassPage::setMoreShow(int i)
{
    moreClassWidget->showMore->setTopName(i,shareData->categoryMap);
    moreClassWidget->showMore->setElementNum(shareData->classElementNumMap);
    moreClassWidget->showMore->setElement(i,shareData->classStrMap);
    moreClassWidget->scrollMore->resize(this->size().width(),this->size().height());
    emit setMore();
}

void ClassPage::testUpdateMap()
{
    if(!shareData->updateStrMap.isEmpty())
    {
        for(auto it = shareData->updateStrMap.begin();it!=shareData->updateStrMap.end();++it)
        {
            qDebug()<<"release id = "<<it.key()<<"  version = "<<it.value().version<<"  packsize = "<<it.value().packageSize;
        }
    }
}

void ClassPage::sendInstallPackage(QString name, int id)
{
    emit installpackage(name,id);
}

void ClassPage::sendUpdatePackage(QString name, int id)
{
    emit updatePackage(name,id);
}

//1:下载 2:更新 3:卸载
void ClassPage::updatePackageStatus(QString name, bool bo,int flag)
{
    qDebug()<<__FUNCTION__<<endl;
    for(auto item = shareData->classStrMap.begin();item != shareData->classStrMap.end();++item)
    {
        if(name == item.value().proName)
        {
          for(int i = 0;i<cateNum;i++)
            {
                if(classWidget[i].getCategory() == item.value().category)
                {
                    for(int j = 0; j < classWidget[i].getElementNum();j++)
                    {
                        if(classWidget[i].getTt(j)->getBtnName()== name)
                        {
                            if(flag == 1 || flag == 2)
                            {
                                classWidget[i].getTt(j)->updateProStatus(bo,flag);
                                if(bo)
                                {
                                    item.value().proStatus = 3;
                                }
                                else
                                {
                                    item.value().proStatus = 4;
                                }
                            }
                            else if(flag == 3)
                            {
                                classWidget[i].getTt(j)->updateProStatus(bo,flag);
                                item.value().proStatus = 1;
                            }
                        }
                    }
                }
            }
        }
    }
}
