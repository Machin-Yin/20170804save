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
#include <QScrollBar>
#include <QSizePolicy>
#include "ClassAndMorePage/recommendwidget.h"

SoftwareCenterWindow::SoftwareCenterWindow(QWidget *parent)
    : QWidget(parent)
{
    resize(980,640);
    vly = new QVBoxLayout;
    titleBar = new TitleBar(this);
    scrollArea  = new QScrollArea(this);
    stackWidget = new QStackedWidget(scrollArea);

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

    pageHome = new HomeWidget(this,shareData,jsonFunc);
    pageClass = new ClassPage(this,jsonFunc,shareData);
    pageUpdate = new UpdatePage(this,jsonFunc,shareData);
    pageManager = new ManagerPage(this,jsonFunc,shareData);
    pageSearch = new SearchWidget(this,shareData,jsonFunc);
    pageDetail = new DetailWidget(this,shareData,jsonFunc);

    setObjectName("mainWindow");
    stackWidget->setObjectName("stackWidget");
    //    pageHome->setObjectName("pageHome");
    //    pageHome->setObjectName("pageHome");
    //    pageHome->setObjectName("pageHome");
    //    pageHome->setObjectName("pageHome");
    //    pageHome->setObjectName("pageHome");

    //scroll area
    stackWidget->setMaximumWidth(1200);
    //    stackWidget->setMaximumSize(960,280);
    //    stackWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //    scrollArea->resize(960,500);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setWidget(stackWidget);
    scrollArea->setWidgetResizable(true);

    stackWidget->addWidget(pageHome);
    stackWidget->addWidget(pageClass);
    stackWidget->addWidget(pageUpdate);
    stackWidget->addWidget(pageManager);
    stackWidget->addWidget(pageSearch);
    //    stackWidget->addWidget(pageSearch->moreWidget);
    stackWidget->addWidget(pageDetail);
    stackWidget->addWidget(pageClass->moreClassWidget);

    pageIndex = 0;
    pageList.append(0);
    createConnections();


}

void SoftwareCenterWindow::createConnections()
{
    connect(titleBar->btnHome,SIGNAL(clicked()),this,SLOT(onBtnHome()));
    connect(titleBar->btnClass,SIGNAL(clicked()),this,SLOT(onBtnClass()));
    connect(titleBar->btnUpdate,SIGNAL(clicked()),this,SLOT(onBtnUpdate()));
    connect(titleBar->btnManager,SIGNAL(clicked()),this,SLOT(onBtnManager()));
    connect(titleBar->leSearch,SIGNAL(returnPressed()),this,SLOT(toPageSearch()));
    connect(this,SIGNAL(toSearch(QString)),pageSearch,SLOT(doSearch(QString)));
    connect(titleBar->btnNext,SIGNAL(clicked()),this,SLOT(nextPage()));
    connect(titleBar->btnBack,SIGNAL(clicked()),this,SLOT(backPage()));
    connect(titleBar->btnRefresh,SIGNAL(clicked()),this,SLOT(refreshPage()));

    connect(stackWidget,SIGNAL(currentChanged(int)),this,SLOT(onPageChanged(int)));
    connect(pageClass,SIGNAL(setMore()),this,SLOT(onBtnMore()));

    connect(this,SIGNAL(refreshPg(int)),pageHome,SLOT(refreshPage(int)));
    connect(pageHome,SIGNAL(toDetailPage(int)),this,SLOT(toPageDetail(int)));
    //    connect(this,SIGNAL(refreshPg(int)),pageClass,SLOT(refreshPage(int)));
    //    connect(this,SIGNAL(refreshPg(int)),pageUpdate,SLOT(refreshPage(int)));
    //    connect(this,SIGNAL(refreshPg(int)),pageManager,SLOT(refreshPage(int)));
    connect(this,SIGNAL(refreshPg(int)),pageSearch,SLOT(refreshPage(int)));
    connect(this,SIGNAL(refreshPg(int)),pageDetail,SLOT(refreshPage(int)));
    //    connect(pageHome,SIGNAL(detailspageSig(int)),this,SLOT(toPageDetail(int)));


    connect(pageUpdate,SIGNAL(theUpdateApp(QString, QString, QString, QString, int, QString, QString)),pageManager,SLOT(onAppUpdate(QString, QString, QString, QString, int, QString, QString)));
    connect(pageUpdate,SIGNAL(appUpdateOk(QString, QString, QString, QString, QString)),pageManager,SLOT(onAppUpdateOk(QString)));
    connect(pageUpdate,SIGNAL(appUpdateFailure(QString)),pageManager,SLOT(onAppUpdateFailure(QString)));
    connect(pageUpdate,SIGNAL(appUpdateOk(QString, QString, QString ,QString, QString)),pageManager,SLOT(upInsToInsd(QString, QString, QString, QString, QString)));
    connect(pageUpdate,SIGNAL(updateStatusChanged()),pageClass,SLOT(resetStatus()));
    connect(pageUpdate,SIGNAL(updatePackageSuccess(QString,bool,int)),pageClass,SLOT(updatePackageStatus(QString,bool,int)));
    connect(pageUpdate,SIGNAL(updatePackageSuccess(QString,bool,int)),pageClass->moreClassWidget->showMore,SLOT(updatePackageStatus(QString,bool,int)));
    connect(pageUpdate,SIGNAL(updatePackage(QString,bool,int)),pageClass,SLOT(updatePackageStatus(QString,bool,int)));
    connect(pageUpdate,SIGNAL(updatePackage(QString,bool,int)),pageClass->moreClassWidget->showMore,SLOT(updatePackageStatus(QString,bool,int)));

    //class page software status connection with update page
    connect(pageManager,SIGNAL(upBtnClicked(QString)),pageUpdate,SLOT(onUpBtnClicked(QString)));
    connect(pageManager,SIGNAL(sigReUpdate(QString,bool,int)),pageClass,SLOT(updatePackageStatus(QString,bool,int)),Qt::QueuedConnection);
    connect(pageManager,SIGNAL(sigReDownload(QString,bool,int)),pageClass,SLOT(updatePackageStatus(QString,bool,int)),Qt::QueuedConnection);
    connect(pageManager,SIGNAL(sigInstallSuccess(QString,bool,int)),pageClass,SLOT(updatePackageStatus(QString,bool,int)),Qt::QueuedConnection);
    connect(pageManager,SIGNAL(sigInstallSuccess(QString,bool,int)),pageClass->moreClassWidget->showMore,SLOT(updatePackageStatus(QString,bool,int)));
    connect(pageManager,SIGNAL(removePackageSuccess(QString,bool,int)),pageClass,SLOT(updatePackageStatus(QString,bool,int)));
    connect(pageManager,SIGNAL(removePackageSuccess(QString,bool,int)),pageClass->moreClassWidget->showMore,SLOT(updatePackageStatus(QString,bool,int)));
    connect(pageManager,SIGNAL(installStatusChanged()),pageClass,SLOT(resetStatus()));
    connect(pageManager,SIGNAL(sigUninstalling(QString,bool,int)),pageClass,SLOT(updatePackageStatus(QString,bool,int)));
    connect(pageManager,SIGNAL(upBtnClicked(QString)),pageUpdate,SLOT(onUpBtnClicked(QString)));

    connect(pageClass,SIGNAL(installpackage(QString,int)),pageManager,SLOT(getInstallRlease(QString, int)),Qt::QueuedConnection);
    connect(pageClass,SIGNAL(updatePackage(QString,int)),pageUpdate,SLOT(onUpBtnClicked(QString)),Qt::QueuedConnection);
    connect(pageClass->moreClassWidget,SIGNAL(installpackage(QString,int)),pageManager,SLOT(getInstallRlease(QString,int)),Qt::QueuedConnection);
    connect(pageClass,SIGNAL(detailspageSig(int)),this,SLOT(toPageDetail(int)));
    connect(pageClass->moreClassWidget,SIGNAL(detailspageSig(int)),this,SLOT(toPageDetail(int)));
    connect(pageClass->moreClassWidget,SIGNAL(updatePackage(QString,int)),pageUpdate,SLOT(onUpBtnClicked(QString)),Qt::QueuedConnection);


    //Detail page software status connection with update page

    connect(pageManager,SIGNAL(sigReUpdate(QString,bool,int)),pageDetail,SLOT(updatePackageStatus(QString,bool,int)),Qt::QueuedConnection);
    connect(pageManager,SIGNAL(sigReDownload(QString,bool,int)),pageDetail,SLOT(updatePackageStatus(QString,bool,int)),Qt::QueuedConnection);
    connect(pageUpdate,SIGNAL(sigUpdateSuccess(QString,bool,int)),pageDetail,SLOT(updatePackageStatus(QString,bool,int)),Qt::QueuedConnection);
    connect(pageManager,SIGNAL(sigInstallSuccess(QString,bool,int)),pageDetail,SLOT(updatePackageStatus(QString,bool,int)),Qt::QueuedConnection);
    //    connect(pageManager,SIGNAL(sigInstallSuccess(QString,bool,int)),pageClass->moreClassWidget->showMore,SLOT(updatePackageStatus(QString,bool,int)));
    //    connect(pageClass->moreClassWidget,SIGNAL(installpackage(QString,int)),pageManager,SLOT(getInstallRlease(QString,int)),Qt::QueuedConnection);
    connect(pageManager,SIGNAL(removePackageSuccess(QString,bool,int)),pageDetail,SLOT(updatePackageStatus(QString,bool,int)));
    //    connect(pageManager,SIGNAL(removePackageSuccess(QString,bool,int)),pageClass->moreClassWidget->showMore,SLOT(updatePackageStatus(QString,bool,int)));
    connect(pageDetail,SIGNAL(updatePackage(QString,int)),pageUpdate,SLOT(onUpBtnClicked(QString)),Qt::QueuedConnection);
    //    connect(pageManager,SIGNAL(installStatusChanged()),pageDetail,SLOT(resetStatus()));
    //    connect(pageUpdate,SIGNAL(updateStatusChanged()),pageDetail,SLOT(resetStatus()));




    //Search page software status connection with update page

    connect(pageManager,SIGNAL(sigReUpdate(QString,bool,int)),pageSearch,SLOT(updatePackageStatus(QString,bool,int)),Qt::QueuedConnection);
    connect(pageManager,SIGNAL(sigReDownload(QString,bool,int)),pageSearch,SLOT(updatePackageStatus(QString,bool,int)),Qt::QueuedConnection);
    connect(pageUpdate,SIGNAL(sigUpdateSuccess(QString,bool,int)),pageSearch,SLOT(updatePackageStatus(QString,bool,int)),Qt::QueuedConnection);
    connect(pageManager,SIGNAL(sigInstallSuccess(QString,bool,int)),pageSearch,SLOT(updatePackageStatus(QString,bool,int)),Qt::QueuedConnection);
    //    connect(pageManager,SIGNAL(sigInstallSuccess(QString,bool,int)),pageClass->moreClassWidget->showMore,SLOT(updatePackageStatus(QString,bool,int)));
    //    connect(pageClass->moreClassWidget,SIGNAL(installpackage(QString,int)),pageManager,SLOT(getInstallRlease(QString,int)),Qt::QueuedConnection);
    connect(pageManager,SIGNAL(removePackageSuccess(QString,bool,int)),pageSearch,SLOT(updatePackageStatus(QString,bool,int)));
    //    connect(pageManager,SIGNAL(removePackageSuccess(QString,bool,int)),pageClass->moreClassWidget->showMore,SLOT(updatePackageStatus(QString,bool,int)));
    //    connect(pageSearch,SIGNAL(updatePackage(QString,int)),pageUpdate,SLOT(onUpBtnClicked(QString)),Qt::QueuedConnection);
    //    connect(pageManager,SIGNAL(installStatusChanged()),pageSearch,SLOT(resetStatus()));
    //    connect(pageUpdate,SIGNAL(updateStatusChanged()),pageSearch,SLOT(resetStatus()));
    //    onBtnHome();
}

SoftwareCenterWindow::~SoftwareCenterWindow()
{
    //    delete shareData;
    //    delete jsonFunc;
}

void SoftwareCenterWindow::onBtnHome()
{
    stackWidget->setCurrentWidget(pageHome);
}

void SoftwareCenterWindow::onBtnClass()
{
    stackWidget->setCurrentWidget(pageClass);
    //    stackWidget->setCurrentWidget(pageDetail);

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
    //    stackWidget->setCurrentWidget(pageSearch->moreWidget);
    emit toSearch(titleBar->leSearch->text());

}

void SoftwareCenterWindow::toPageDetail(int productId)
{
    qDebug()<<"++++++++++++++ productId="<<productId<<endl;
    stackWidget->setCurrentWidget(pageDetail);
    pageDetail->toPage(productId);
}

void SoftwareCenterWindow::nextPage()
{
    qDebug()<<__FUNCTION__<<"pagelist.count()="<<pageList.count()<<endl;

    if(pageIndex < pageList.count()-1){
        pageIndex++;
        qDebug()<<"pageIndex="<<pageIndex<<endl;
        disconnect(stackWidget,SIGNAL(currentChanged(int)),this,SLOT(onPageChanged(int)));
        stackWidget->setCurrentIndex(pageList.at(pageIndex));
//        pageList.removeLast();
        connect(stackWidget,SIGNAL(currentChanged(int)),this,SLOT(onPageChanged(int)));

    }

    showSearchImg();

}

void SoftwareCenterWindow::backPage()
{
    qDebug()<<__FUNCTION__<<"pagelist.count()="<<pageList.count()<<endl;

    if(pageIndex > 0){
        pageIndex--;
        qDebug()<<"pageIndex="<<pageIndex<<endl;
        disconnect(stackWidget,SIGNAL(currentChanged(int)),this,SLOT(onPageChanged(int)));
        stackWidget->setCurrentIndex(pageList.at(pageIndex));
        if(!titleBar->btnNext->isEnabled())
            titleBar->btnNext->setDisabled(false);
//        pageList.removeLast();

        connect(stackWidget,SIGNAL(currentChanged(int)),this,SLOT(onPageChanged(int)));

    }

    showSearchImg();
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
    qDebug()<<__FUNCTION__<<"page changed index = "<< index <<endl;
    scrollArea->verticalScrollBar()->setValue(0);
    pageList.append(index);
    pageIndex = pageList.count()-1;

    qDebug()<<"pagelist.count()="<<pageList.count()<<endl;
    if(pageList.count() <= 1){
        titleBar->btnBack->setDisabled(true);
        titleBar->btnNext->setDisabled(true);
    }else{
        titleBar->btnBack->setDisabled(false);
    }

    showSearchImg();

}

void SoftwareCenterWindow::showSearchImg()
{
    if(stackWidget->currentWidget() != pageSearch ){
        titleBar->leSearch->setText("");
        titleBar->lbSearchImg->show();
        titleBar->lbSearch->show();
    }
}
