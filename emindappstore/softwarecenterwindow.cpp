#include "softwarecenterwindow.h"
#include "titlebar.h"
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>
#include "homewidget.h"
#include <QGridLayout>
#include "searchwidget.h"
#include "detailwidget.h"
#include <QScrollArea>
#include "ClassAndMorePage/classpage.h"
#include "managerpage/managerpage.h"
#include "updatepage/updatepage.h"

SoftwareCenterWindow::SoftwareCenterWindow(QWidget *parent)
    : QWidget(parent)
{
    resize(960,640);
    vly = new QVBoxLayout;
    titleBar = new TitleBar(this);
    scrollArea  = new QScrollArea(this);
    stackWidget = new QStackedWidget;

    /*cannot hide the title and icon*/
    flags = 0;
    flags |= Qt::CustomizeWindowHint;
    flags |= Qt::WindowMinimizeButtonHint;
    flags |= Qt::WindowMaximizeButtonHint;
    flags |= Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

    vly->setContentsMargins(0,0,0,0);
    vly->setSpacing(0);
    vly->addWidget(titleBar);
    vly->addWidget(scrollArea);
    this->setLayout(vly);

    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setWidget(stackWidget);
    scrollArea->setVisible(true);
    scrollArea->setWidgetResizable(true);
    scrollArea->ensureWidgetVisible(stackWidget);

    shareData = new ShareData();
    jsonFunc = new JSONFUNC(shareData);

    pageHome = new HomeWidget(this);
    pageClass = new ClassPage(this,jsonFunc,shareData);
    pageUpdate = new UpdatePage(this,jsonFunc);
    pageManager = new ManagerPage(this,jsonFunc);

    pageSearch = new SearchWidget(this);
    pageDetail = new DetailWidget(this);

    stackWidget->addWidget(pageHome);
    stackWidget->addWidget(pageClass);
    stackWidget->addWidget(pageUpdate);
    stackWidget->addWidget(pageManager);
    stackWidget->addWidget(pageSearch);
    stackWidget->addWidget(pageDetail);
    stackWidget->addWidget(pageClass->moreClassWidget);

    pageIndex = 0;

    connect(titleBar->btnHome,SIGNAL(clicked()),this,SLOT(onBtnHome()));
    connect(titleBar->btnClass,SIGNAL(clicked()),this,SLOT(onBtnClass()));
    connect(titleBar->btnUpdate,SIGNAL(clicked()),this,SLOT(onBtnUpdate()));
    connect(titleBar->btnManager,SIGNAL(clicked()),this,SLOT(onBtnManager()));
    connect(titleBar->leSearch,SIGNAL(returnPressed()),this,SLOT(toPageSearch()));
    connect(this,SIGNAL(toSearch(QString)),pageSearch,SLOT(doSearch(QString)));
    connect(titleBar->btnNext,SIGNAL(clicked()),this,SLOT(nextPage()));
    connect(titleBar->btnBack,SIGNAL(clicked()),this,SLOT(backPage()));
    connect(titleBar->btnRefresh,SIGNAL(clicked()),this,SLOT(refreshPage()));
//    connect(this,SIGNAL(pageChanged()),this,SLOT(onPageChanged()));
    connect(stackWidget,SIGNAL(currentChanged(int)),this,SLOT(onPageChanged(int)));
    connect(pageClass,SIGNAL(setMore()),this,SLOT(onBtnMore()));

    connect(stackWidget,SIGNAL(refreshPg(int)),pageHome,SLOT(refreshPage(int)));
    connect(stackWidget,SIGNAL(refreshPg(int)),pageClass,SLOT(refreshPage(int)));
    connect(stackWidget,SIGNAL(refreshPg(int)),pageUpdate,SLOT(refreshPage(int)));
    connect(stackWidget,SIGNAL(refreshPg(int)),pageManager,SLOT(refreshPage(int)));
    connect(stackWidget,SIGNAL(refreshPg(int)),pageSearch,SLOT(refreshPage(int)));
    connect(stackWidget,SIGNAL(refreshPg(int)),pageDetail,SLOT(refreshPage(int)));

    connect(pageUpdate,SIGNAL(theUpdateApp(QString, QString, QString, QString)),pageManager,SLOT(onAppUpdate(QString, QString, QString, QString)));
    connect(pageUpdate,SIGNAL(appUpdateOk(QString, QString, QString)),pageManager,SLOT(onAppUpdateOk(QString)));
    connect(pageUpdate,SIGNAL(appUpdateFailure(QString)),pageManager,SLOT(onAppUpdateFailure(QString)));
    connect(pageUpdate,SIGNAL(appUpdateOk(QString, QString, QString)),pageManager,SLOT(updToInsd(QString, QString, QString)));
    connect(pageManager,SIGNAL(insdBtnClicked(QString)),pageUpdate,SLOT(onInsdBtnClicked(QString)));

    connect(pageClass,SIGNAL(installpackage(QString,int)),pageManager,SLOT(getInstallRlease(QString, int)),Qt::QueuedConnection);
    connect(pageManager,SIGNAL(sigInstallSuccess(QString,bool,int)),pageClass,SLOT(updatePackageStatus(QString,bool,int)),Qt::QueuedConnection);
    connect(pageManager,SIGNAL(sigInstallSuccess(QString,bool,int)),pageClass->moreClassWidget->showMore,SLOT(updatePackageStatus(QString,bool,int)));
    connect(pageClass->moreClassWidget,SIGNAL(installpackage(QString,int)),pageManager,SLOT(getInstallRlease(QString,int)),Qt::QueuedConnection);
    connect(pageManager,SIGNAL(removePackageSuccess(QString,bool,int)),pageClass,SLOT(updatePackageStatus(QString,bool,int)));
    connect(pageManager,SIGNAL(removePackageSuccess(QString,bool,int)),pageClass->moreClassWidget->showMore,SLOT(updatePackageStatus(QString,bool,int)));

    connect(pageClass,SIGNAL(updatePackage(QString,int)),pageUpdate,SLOT(onInsdBtnClicked(QString)),Qt::QueuedConnection);

    connect(pageManager,SIGNAL(installStatusChanged()),pageClass,SLOT(resetStatus()));
    connect(pageUpdate,SIGNAL(updateStatusChanged()),pageClass,SLOT(resetStatus()));
    //    onBtnHome();

}

SoftwareCenterWindow::~SoftwareCenterWindow()
{

}

void SoftwareCenterWindow::onBtnHome()
{
    stackWidget->setCurrentWidget(pageHome);
}

void SoftwareCenterWindow::onBtnClass()
{
    stackWidget->setCurrentWidget(pageClass);

}

void SoftwareCenterWindow::onBtnMore()
{
    stackWidget->setCurrentWidget(pageClass->moreClassWidget);

}

void SoftwareCenterWindow::onBtnUpdate()
{
    stackWidget->setCurrentWidget(pageUpdate);
}

void SoftwareCenterWindow::onBtnManager()
{
    stackWidget->setCurrentWidget(pageManager);

}

void SoftwareCenterWindow::mousePressEvent(QMouseEvent* event)
{
    setFocus();
}

void SoftwareCenterWindow::toPageSearch()
{
    stackWidget->setCurrentWidget(pageSearch);
    emit toSearch(titleBar->leSearch->text());

}

void SoftwareCenterWindow::toPageDetail()
{
    stackWidget->setCurrentWidget(pageDetail);
}

void SoftwareCenterWindow::nextPage()
{
    pageIndex += 1;
    stackWidget->setCurrentIndex(pageList.at(pageIndex));
}

void SoftwareCenterWindow::backPage()
{
    pageIndex -= 1;
    stackWidget->setCurrentIndex(pageList.at(pageIndex));
}

void SoftwareCenterWindow::refreshPage()
{
    if(stackWidget->currentIndex() == 1 )
        emit refreshPg(1);
    if(stackWidget->currentIndex() == 1 )
        emit refreshPg(2);
    if(stackWidget->currentIndex() == 1 )
        emit refreshPg(3);
    if(stackWidget->currentIndex() == 1 )
        emit refreshPg(4);
    if(stackWidget->currentIndex() == 1 )
        emit refreshPg(5);
    if(stackWidget->currentIndex() == 1 )
        emit refreshPg(6);
    if(stackWidget->currentIndex() == 1 )
        emit refreshPg(7);

}

void SoftwareCenterWindow::onPageChanged(int index)
{
    qDebug()<<"page changed index = "<< index <<endl;
    pageList << index;
    if(pageList.count() < 2){
        titleBar->btnBack->setDisabled(true);
        titleBar->btnNext->setDisabled(true);
    }

    if(pageIndex < 1){
        titleBar->btnBack->setDisabled(false);
    }

    if(pageIndex > pageList.count()){
        titleBar->btnNext->setDisabled(false);
    }

}
