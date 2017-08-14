#include <QHeaderView>
#include "managerpage.h"

ManagerPage::ManagerPage(QWidget *parent, JSONFUNC *json) : QWidget(parent)
{
    updm = new PkUpdates(this,json);
    jsonFunc = json;
//    updm->getMagData();
    connect(jsonFunc,SIGNAL(productIsOk()),this,SLOT(getPackageInstalled()));
    connect(updm,SIGNAL(getInsFinished(QVariantMap)),this,SLOT(onGetInsFinished(QVariantMap)));
    connect(jsonFunc,SIGNAL(updateIsOk()),this,SLOT(onAppInstall()));
    connect(this,SIGNAL(appInstallOk(QString, QString, QString)),this,SLOT(updToInsd(QString, QString, QString)));
    connect(updm,SIGNAL(installStatusChanged()),this,SLOT(onStatusChanged()));

    createManagerWindow();
}


void ManagerPage::createManagerWindow()
{
    manScroArea = new QScrollArea(this);
    manScroArea->resize(this->width(),this->height());
    manScroArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    manScroArea->setFrameShape(QFrame::NoFrame);

    QString strtask = "进行中的任务";
    QString stronekey =  "全部开始";
    QWidget *wid = new QWidget();
    manTaskBar = new TaskBar(this,strtask,stronekey);
    connect(manTaskBar->getOnekeyBtn(),SIGNAL(clicked()),this,SLOT(allStartBtnclicked()));

//    manTaskBar->size().setHeight(64);

    installTable = new QTableWidget();
    installTable->setColumnCount(1);
    installTable->setRowCount(0);
    installTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    installTable->setFrameShape(QFrame::NoFrame);
    installTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    installTable->setFocusPolicy(Qt::NoFocus);
    installTable->resizeColumnToContents (0);
    installTable->setShowGrid(false);
    installTable->setStyleSheet(
                "QTableWidget {"
                "background-color: #00000000;"
                "}"
                "QTableWidget::item {"
                "border-top: 1px solid #dcdcdc;"
                "}"
                "QTableWidget::item:selected{"
                "background-color: white;"
                "}"
                );
    installTable->verticalHeader()->setDefaultSectionSize(96);
    installTable->horizontalHeader()->setStretchLastSection(true);
    QHeaderView* vheaderView = installTable->verticalHeader();
    vheaderView->setHidden(true);
    QHeaderView *hheaderView = installTable->horizontalHeader();
    hheaderView->setHidden(true);

    int rowCount = installTable->rowCount();
    installTable->setMinimumHeight(96*rowCount);

    splitLabel = new QLabel();
    splitLabel->size().setHeight(16);
    splitLabel->setStyleSheet("background: #f9f9f9;");

    QString strtask1 = "已完成的任务 ";
    QString stronekey1 =  "";
    manCompBar = new TaskBar(this,strtask1,stronekey1);
    manCompBar->hideOnekeyButton();
    manCompBar->size().setHeight(64);

    compTable = new QTableWidget();
    compTable->setColumnCount(1);
    //    compTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    compTable->setFrameShape(QFrame::NoFrame);
    compTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    compTable->setFocusPolicy(Qt::NoFocus);
    compTable->resizeColumnToContents (0);
    compTable->setShowGrid(false);
    compTable->setStyleSheet(
                "QTableWidget {"
                "background-color: #00000000;"
                "}"
                "QTableWidget::item {"
                "border-top: 1px solid #dcdcdc;"
                "}"
                "QTableWidget::item:selected{"
                "background:white;"
                "}"
                );
    compTable->verticalHeader()->setDefaultSectionSize(96);
    compTable->horizontalHeader()->setStretchLastSection(true);
    QHeaderView* vheaderView1 = compTable->verticalHeader();
    vheaderView1->setHidden(true);
    QHeaderView *hheaderView1 = compTable->horizontalHeader();
    hheaderView1->setHidden(true);

    vboxLayout = new QVBoxLayout();
    vboxLayout->setMargin(0);
    vboxLayout1 = new QVBoxLayout();
    vboxLayout1->setContentsMargins(16,0,16,0);
    vboxLayout2 = new QVBoxLayout();
    vboxLayout2->setContentsMargins(16,0,16,0);

    vboxLayout->addLayout(vboxLayout1);
    vboxLayout1->addSpacing(18);
    vboxLayout1->addWidget(manTaskBar);
    vboxLayout1->addSpacing(18);
    vboxLayout1->addWidget(installTable);
    vboxLayout->addWidget(splitLabel);
    vboxLayout->addLayout(vboxLayout2);
    //    vboxLayout->addSpacing(0);
    vboxLayout2->addSpacing(24);
    vboxLayout2->addWidget(manCompBar);
    vboxLayout2->addSpacing(24);
    vboxLayout2->addWidget(compTable);
    //    vboxLayout->addStretch();

    wid->setLayout(vboxLayout);
    manScroArea->setWidget(wid);
    manScroArea->setWidgetResizable(true);

}

void ManagerPage::onAppUpdate(QString iconUrl, QString appName, QString appVersion, QString appSize)
{
    qDebug() << __FUNCTION__;

    for(int i = 0; i < installTable->rowCount(); i++)
    {
        ManagerWidget *managerWidget = (ManagerWidget *)installTable->cellWidget(i, 0);
        QString insdName = managerWidget->getButton(NAMEBUTTON)->text();
        if(appName == insdName)
        {
            managerWidget->setManagerButton(tr("Updating..."));
            return;
        }
    }

    ManagerWidget *manTaskManager = new ManagerWidget(this,iconUrl,appName,appVersion,appSize);
    manTaskManager->setManagerButton(tr("Updating..."));
    manTaskManager->getButton(UNINSBUTTON)->hide();
    installTable->insertRow(0);
    installTable->setCellWidget(0,0,manTaskManager);
    int rowCount = installTable->rowCount();
    installTable->setMinimumHeight(96*rowCount);
    connect(manTaskManager->getButton(MANAGERBUTTON),SIGNAL(clicked()),this,SLOT(insManBtnClicked()));
}

void ManagerPage::onAppUpdateOk(QString appName)
{
    int rowCount = installTable->rowCount();
    for(int i = 0; i < rowCount; i++)
    {
        ManagerWidget *insWidget = (ManagerWidget *)installTable->cellWidget(i, 0);
        QString upAppName = insWidget->getButton(NAMEBUTTON)->text();
        if(appName == upAppName)
        {
            installTable->removeRow(i);
            installTable->setMinimumHeight(96*(rowCount - 1));
        }
    }
}

void ManagerPage::onAppUpdateFailure(QString appName)
{
    int rowCount = installTable->rowCount();
    for(int i = 0; i < rowCount; i++)
    {
        ManagerWidget *insWidget = (ManagerWidget *)installTable->cellWidget(i, 0);
        QString upAppName = insWidget->getAppName();
        if(appName == upAppName)
        {
            insWidget->getButton(MANAGERBUTTON)->setText(tr("reupdate"));
        }
    }
}

void ManagerPage::onAppInstallSuccess(QString appName)
{
    int rowCount = installTable->rowCount();
    for(int i = 0; i < rowCount; i++)
    {
        ManagerWidget *insWidget = (ManagerWidget *)installTable->cellWidget(i, 0);
        QString insAppName = insWidget->getAppName();
        if(appName == insAppName)
        {
            QString headUrl = insWidget->getHeadUrl();
            QString appVersion = insWidget->getVersion();
            emit appInstallOk(appName,headUrl,appVersion);
            emit sigInstallSuccess(appName,true,1);
            installTable->removeRow(i);
            installTable->setMinimumHeight(96*(rowCount - 1));
//            insWidget->getButton(MANAGERBUTTON)->setText(tr("Install Success..."));

        }
    }
}

void ManagerPage::onAppInstallFailure(QString appName)
{
    int rowCount = installTable->rowCount();
    for(int i = 0; i < rowCount; i++)
    {
        ManagerWidget *insWidget = (ManagerWidget *)installTable->cellWidget(i, 0);
        QString insAppName = insWidget->getAppName();
        if(appName == insAppName)
        {
            emit sigInstallSuccess(appName,false,1);
            insWidget->getButton(MANAGERBUTTON)->setText(tr("reinstall"));
        }
    }
}

void ManagerPage::updToInsd(QString appName, QString iconUrl, QString appVer)
{
    qDebug() << __FUNCTION__;
    ManagerWidget *manCompManager = new ManagerWidget(this,iconUrl,appName,appVer,"");
    compTable->insertRow(0);
    compTable->setCellWidget(0,0,manCompManager);
    int rowCount = compTable->rowCount();
    compTable->setMinimumHeight(96*rowCount);
    manCompManager->resolveNameToId(appName);
    connect(manCompManager,SIGNAL(resetPackageIdSuccess(QString)),manCompManager,SLOT(setInstallSize(QString)));
    connect(manCompManager->getButton(HEADBUTTON),SIGNAL(clicked()),this,SLOT(compBtnClicked()));
    connect(manCompManager->getButton(NAMEBUTTON),SIGNAL(clicked()),this,SLOT(compBtnClicked()));
    connect(manCompManager->getButton(UNINSBUTTON),SIGNAL(clicked()),this,SLOT(compBtnClicked()));
    connect(manCompManager->getButton(MANAGERBUTTON),SIGNAL(clicked()),this,SLOT(compBtnClicked()));
}

void ManagerPage::deleteRmvRow(QString pacId)
{
    for(int i = 0; i < compTable->rowCount(); i++)
    {
        ManagerWidget* managerWidget = (ManagerWidget*)compTable->cellWidget(i,0);
        if(pacId == managerWidget->getPkgId())
        {
            QString pacName = managerWidget->getAppName();
            emit removePackageSuccess(pacName,true,3);
            compTable->removeRow(i);
        }
    }
}

void ManagerPage::getPackageInstalled()
{
    updm->getMagData();
}

void ManagerPage::insManBtnClicked()
{
    int rowCount = installTable->rowCount();
    for(int i = 0; i < rowCount; i++)
    {
        ManagerWidget *insWidget = (ManagerWidget *)installTable->cellWidget(i, 0);
        if(sender() == insWidget->getButton(MANAGERBUTTON))
        {
            emit insdBtnClicked(insWidget->getButton(NAMEBUTTON)->toolTip());
        }
    }
}

void ManagerPage::allStartBtnclicked()
{
    int insdCount = installTable->rowCount();
    for(int i = 0; i < insdCount; i++)
    {
        ManagerWidget* insWidget = (ManagerWidget*)installTable->cellWidget(i,0);

        emit insdBtnClicked(insWidget->getButton(NAMEBUTTON)->toolTip());
    }
}

void ManagerPage::getInstallRlease(QString appName, int releaseId)
{
    qDebug() << __FUNCTION__;
    Q_UNUSED(appName);
    int releaseAry[1];
    releaseAry[0] = releaseId;

    jsonFunc->jsonData->updateStrMap.clear();
    jsonFunc->getUpdateRelease(releaseAry,1);
}

void ManagerPage::onAppInstall()
{
    if(jsonFunc->jsonData->updateStrMap.isEmpty())
    {
        return;
    }
    QString headUrl = jsonFunc->jsonData->updateStrMap.begin().value().proImage;
    QString appName = jsonFunc->jsonData->updateStrMap.begin().value().proName;
    QString appVersion = tr("Version :") + jsonFunc->jsonData->updateStrMap.begin().value().proName;
    double appSize = jsonFunc->jsonData->updateStrMap.begin().value().packageSize;
    QString pacSize = updm->transPackSize(appSize);
    QString appSizeStr = tr("Size :") + pacSize;

    ManagerWidget *manTaskManager = new ManagerWidget(this,headUrl, appName, appVersion, appSizeStr);
    manTaskManager->setManagerButton(tr("Installing..."));
    manTaskManager->getButton(UNINSBUTTON)->hide();
    installTable->insertRow(0);
    installTable->setCellWidget(0,0,manTaskManager);
    int rowCount = installTable->rowCount();
    installTable->setMinimumHeight(96*rowCount);

    connect(updm,SIGNAL(installSuccess()),manTaskManager,SLOT(onInstallSuccess()));
    connect(updm,SIGNAL(installFailure()),manTaskManager,SLOT(onInstallFailure()));
    connect(manTaskManager,SIGNAL(appInstallSuccess(QString)),this,SLOT(onAppInstallSuccess(QString)));
    connect(manTaskManager,SIGNAL(appInstallFailure(QString)),this,SLOT(onAppInstallFailure(QString)));

    updm->installPackage(appName);
//    connect(manTaskManager->getButton(MANAGERBUTTON),SIGNAL(clicked()),this,SLOT(insManBtnClicked()));
}

void ManagerPage::onGetInsFinished(QVariantMap insdMap)
{
    int insRowCount = insdMap.count();

    compTable->setRowCount(insRowCount);
    compTable->setMinimumHeight(96*insRowCount);

    QVariantMap::iterator item;
    int count = 0;
    for(item = insdMap.begin(); item != insdMap.end(); item++)
    {
        //        qDebug() << "item.value().proName" << ;
        QString packageId = item.key();
        QString appName = PackageKit::Transaction::packageName(packageId);
        QString appVersion = tr("Version : V") + PackageKit::Transaction::packageVersion(packageId);
        QString headUrl = item.value().toString();

        //        QString appVersion = item.value().
        ManagerWidget *manTaskManager = new ManagerWidget(this,headUrl,appName,appVersion,"");
        manTaskManager->setPkgId(packageId);
        manTaskManager->getButton(MANAGERBUTTON)->setText(tr("Open"));
        compTable->setCellWidget(count,0,manTaskManager);
        manTaskManager->setInstallSize(packageId);
        count ++;

        connect(manTaskManager->getButton(HEADBUTTON),SIGNAL(clicked()),this,SLOT(compBtnClicked()));
        connect(manTaskManager->getButton(NAMEBUTTON),SIGNAL(clicked()),this,SLOT(compBtnClicked()));
        connect(manTaskManager->getButton(UNINSBUTTON),SIGNAL(clicked()),this,SLOT(compBtnClicked()));
        connect(manTaskManager->getButton(MANAGERBUTTON),SIGNAL(clicked()),this,SLOT(compBtnClicked()));

    }
}

void ManagerPage::compBtnClicked()
{
    for(int i = 0; i < compTable->rowCount(); i++)
    {
        ManagerWidget* managerWidget = (ManagerWidget*)compTable->cellWidget(i,0);

        if(sender() == managerWidget->getButton(HEADBUTTON) )
        {
            qDebug() << "btn == headButton:" << "row == " << i;
            break;
        }
        else if(sender() == managerWidget->getButton(NAMEBUTTON))
        {
            qDebug() << "btn == nameButton:" << "row == " << i;
            break;
        }
        else if(sender() == managerWidget->getButton(UNINSBUTTON) )
        {
            qDebug() << "btn == uninsButton:" << "row == " << i;
            QString packageID = managerWidget->getPkgId();
            managerWidget->removePackage(packageID);
            connect(managerWidget,SIGNAL(pacRmvSuccess(QString)),this,SLOT(deleteRmvRow(QString)));

            break;
        }
        else if(sender() == managerWidget->getButton(MANAGERBUTTON))
        {
            qDebug() << "btn == managerButton:" << "row == " << i;
            QString openAppName =  PackageKit::Transaction::packageName(managerWidget->getPkgId());
            QProcess *proc = new QProcess;
            proc->start(openAppName);
            break;
        }

    }
}

void ManagerPage::onStatusChanged()
{
    emit installStatusChanged();
}

bool ManagerPage::event(QEvent *event)
{
    if(event->type() == QEvent::Resize)
    {
        manScroArea->resize(QSize(this->width(),this->height()));
        return true;
    }
    return QWidget::event(event);

}


