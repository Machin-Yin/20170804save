#include <QHeaderView>
#include <QSettings>
#include "managerpage.h"

ManagerPage::ManagerPage(QWidget *parent, JSONFUNC *json ,ShareData *sharedata) : QWidget(parent)
{
    updm = new PkUpdates(this,json,sharedata);
    shareData = new ShareData();
    jsonFunc = new JSONFUNC(shareData);
//    updm->getMagData();
    connect(json,SIGNAL(productIsOk()),this,SLOT(getPackageInstalled()));
    connect(updm,SIGNAL(getInsFinished(QMap<QString,INSTALLEDSTRUCT>)),this,SLOT(onGetInsFinished(QMap<QString,INSTALLEDSTRUCT>)));
//    connect(jsonFunc,SIGNAL(updateIsOk()),this,SLOT(onAppInstall()));
    connect(this,SIGNAL(appInstallOk(QString, QString, QString,QString,QString)),this,SLOT(upInsToInsd(QString, QString, QString,QString,QString)));
    connect(updm,SIGNAL(installStatusChanged()),this,SLOT(onStatusChanged()));
    connect(this,SIGNAL(insBtnClicked(QString,int)),this,SLOT(getInstallRlease(QString, int)),Qt::QueuedConnection);
    connect(this,SIGNAL(insBtnClicked(QString,int)),this,SLOT(sendSigReDownload(QString)),Qt::QueuedConnection);
    connect(this,SIGNAL(upBtnClicked(QString)),this,SLOT(sendSigReUpdate(QString)),Qt::QueuedConnection);

    createManagerWindow();
}


void ManagerPage::createManagerWindow()
{
    manScroArea = new QScrollArea(this);
    manScroArea->resize(this->width(),this->height());
    manScroArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    manScroArea->setFrameShape(QFrame::NoFrame);

    QString strtask = tr("Tasks In Progress");
    QString stronekey =  tr("All Start");
    QWidget *wid = new QWidget();
    manTaskBar = new TaskBar(this,strtask,stronekey);
    connect(manTaskBar->getOnekeyBtn(),SIGNAL(clicked()),this,SLOT(allStartBtnclicked()));

//    manTaskBar->size().setHeight(64);

    installTable = new QTableWidget();
    installTable->setColumnCount(1);
    installTable->setRowCount(0);
    installTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    installTable->setFrameShape(QFrame::NoFrame);
    installTable->setSelectionMode(QAbstractItemView::NoSelection);
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

    readConfigFile();

//    int rowCount = installTable->rowCount();
//    installTable->setMinimumHeight(96*rowCount);

    splitLabel = new QLabel();
    splitLabel->size().setHeight(16);
    splitLabel->setStyleSheet("background: #f9f9f9;");

    QString strtask1 = tr("Completed Tasks ");
    QString stronekey1 =  "";
    manCompBar = new TaskBar(this,strtask1,stronekey1);
    manCompBar->hideOnekeyButton();
    manCompBar->size().setHeight(64);

    compTable = new QTableWidget();
    compTable->setColumnCount(1);
    //    compTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    compTable->setFrameShape(QFrame::NoFrame);
    compTable->setSelectionMode(QAbstractItemView::NoSelection);
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
    wid->setObjectName("pageManagerWidget");
    manScroArea->setWidget(wid);
    manScroArea->setWidgetResizable(true);

}

void ManagerPage::onAppUpdate(QString iconUrl, QString appName, QString appVersion, QString appSize, int relid, QString exefile, QString pkgname)
{
    qDebug() << __FUNCTION__;

    for(int i = 0; i < installTable->rowCount(); i++)
    {
        ManagerWidget *managerWidget = (ManagerWidget *)installTable->cellWidget(i, 0);
        QString insName = managerWidget->getPkgName();
        if(pkgname == insName)
        {
            managerWidget->setManagerButton(tr("Updating..."));
            return;
        }
    }

    ManagerWidget *manTaskManager = new ManagerWidget(this,iconUrl,appName,appVersion,appSize);
    manTaskManager->getButton(MANAGERBUTTON)->setStyleSheet("font-weight: 500; color:#027aff; border: 1px solid #027aff  ; border-radius: 2px; ");

    manTaskManager->setFlag("UPDATEFLAG");
    manTaskManager->setReleaseId(relid);
    manTaskManager->setPkgName(pkgname);
    manTaskManager->setExeFile(exefile);
    manTaskManager->setManagerButton(tr("Updating..."));
    manTaskManager->getButton(UNINSBUTTON)->hide();
    installTable->insertRow(0);
    installTable->setCellWidget(0,0,manTaskManager);
    int rowCount = installTable->rowCount();
    installTable->setMinimumHeight(96*rowCount);
    connect(manTaskManager->getButton(MANAGERBUTTON),SIGNAL(clicked()),this,SLOT(insManBtnClicked()));

    QString flag = "UPDATEFLAG";
    saveConfigFile(iconUrl, appName, appVersion, appSize, flag, exefile, relid, pkgname);

}

void ManagerPage::onAppUpdateOk(QString appName)
{
    int rowCount = installTable->rowCount();
    for(int i = 0; i < rowCount; i++)
    {
        ManagerWidget *insWidget = (ManagerWidget *)installTable->cellWidget(i, 0);
        QString upAppName = insWidget->getAppName();
        if(appName == upAppName)
        {
            installTable->removeRow(i);
            installTable->setMinimumHeight(96*(rowCount - 1));

            int releaseId = insWidget->getReleaseId();
            QString section = QString::number(releaseId,10);
            QSettings settings("upins.ini", QSettings::IniFormat);
            settings.remove(section);
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
            insWidget->getButton(MANAGERBUTTON)->setText(tr("ReUpdate"));
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
            QString exefile = insWidget->getExeFile();
            QString packageName = insWidget->getPkgName();
            int releaseId = insWidget->getReleaseId();
            QString section = QString::number(releaseId,10);
            emit appInstallOk(appName,headUrl,appVersion,exefile,packageName);
            emit sigInstallSuccess(appName,true,DOWNLOAD);
            installTable->removeRow(i);
            installTable->setMinimumHeight(96*(rowCount - 1));

            QSettings settings("upins.ini", QSettings::IniFormat);
            settings.remove(section);

            return;
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
            emit sigInstallSuccess(appName,false,DOWNLOAD);
            insWidget->getButton(MANAGERBUTTON)->setText(tr("ReDownLoad"));
        }
    }
}

void ManagerPage::upInsToInsd(QString appName, QString iconUrl, QString appVer, QString exefile, QString pkgName)
{
    qDebug() << __FUNCTION__;
    int rowCountInsd = compTable->rowCount();
    for(int i = 0; i < rowCountInsd; i++)
    {
        ManagerWidget *insdWidget = (ManagerWidget *)compTable->cellWidget(i, 0);
        QString insdAppName = insdWidget->getAppName();
        if(appName == insdAppName)
        {
            qDebug() << "remove i == " <<  i;
            compTable->removeRow(i);
            compTable->setMinimumHeight(96*(rowCountInsd - 1));
            break;
        }
    }
    ManagerWidget *manCompManager = new ManagerWidget(this,iconUrl,appName,appVer,"");
    manCompManager->setExeFile(exefile);
    compTable->insertRow(0);
    compTable->setCellWidget(0,0,manCompManager);
    int rowCount = compTable->rowCount();
    compTable->setMinimumHeight(96*rowCount);
    manCompManager->resolveNameToId(pkgName);
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
            emit removePackageSuccess(pacName,true,UNINSTALL);
            compTable->removeRow(i);
        }
    }
}

void ManagerPage::onRemoveFailure(QString pacId)
{
    for(int i = 0; i < compTable->rowCount(); i++)
    {
        ManagerWidget* managerWidget = (ManagerWidget*)compTable->cellWidget(i,0);
        if(pacId == managerWidget->getPkgId())
        {
            QString appName = managerWidget->getAppName();
            emit removePackageSuccess(appName,false,UNINSTALL);
            managerWidget->getButton(UNINSBUTTON)->setText(tr("UnInstall"));
        }
    }
}

void ManagerPage::getPackageInstalled()
{
    updm->getMagData();
}

void ManagerPage::insManBtnClicked()
{
    qDebug() << __FUNCTION__;
    int rowCount = installTable->rowCount();
    for(int i = 0; i < rowCount; i++)
    {
        ManagerWidget *insWidget = (ManagerWidget *)installTable->cellWidget(i, 0);
        if(sender() == insWidget->getButton(MANAGERBUTTON))
        {
            QString flag = insWidget->getFlag();
            if(flag == "UPDATEFLAG")
            {
                insWidget->setManagerButton(tr("Updating..."));
                emit upBtnClicked(insWidget->getAppName());
            }
            else if(flag == "INSTALLFLAG")
            {
                int releaseId = insWidget->getReleaseId();
                qDebug() << "releaseId == " << releaseId;
                insWidget->setManagerButton(tr("Installing..."));
                emit insBtnClicked(insWidget->getAppName(),releaseId);
            }
        }
    }
}

void ManagerPage::allStartBtnclicked()
{
    int insdCount = installTable->rowCount();
    for(int i = 0; i < insdCount; i++)
    {
        ManagerWidget* insUpWidget = (ManagerWidget*)installTable->cellWidget(i,0);
        QString flag = insUpWidget->getFlag();
        if(flag == "UPDATEFLAG")
        {
            insUpWidget->setManagerButton(tr("Updating..."));
            emit upBtnClicked(insUpWidget->getButton(NAMEBUTTON)->text());
        }
        else if(flag == "INSTALLFLAG")
        {
            int releaseId = insUpWidget->getReleaseId();
            insUpWidget->setManagerButton(tr("Installing..."));
            emit insBtnClicked(insUpWidget->getButton(NAMEBUTTON)->text(),releaseId);
        }
    }
}

void ManagerPage::getInstallRlease(QString appName, int releaseId)
{
    qDebug() << __FUNCTION__;    
    for(int i = 0; i < installTable->rowCount(); i++)
    {
        ManagerWidget *managerWidget = (ManagerWidget *)installTable->cellWidget(i, 0);
        QString insName = managerWidget->getAppName();
        if(appName == insName)
        {
            QString packageName = managerWidget->getPkgName();
            managerWidget->setManagerButton(tr("Installing..."));
            managerWidget->installPackage(packageName);
            return;
        }
    }
    ManagerWidget *manTaskManager = new ManagerWidget(this,"", appName, "", "");
    manTaskManager->setReleaseId(releaseId);
    manTaskManager->getButton(UNINSBUTTON)->hide();
    manTaskManager->setFlag("INSTALLFLAG");

    installTable->insertRow(0);
    installTable->setCellWidget(0,0,manTaskManager);
    int rowCount = installTable->rowCount();
    installTable->setMinimumHeight(96*rowCount);

    connect(manTaskManager->jsonFunc,SIGNAL(updateIsOk()),this,SLOT(onAppInstall()));

    int releaseAry[1];
    releaseAry[0] = releaseId;
    manTaskManager->jsonFunc->getUpdateRelease(releaseAry,1);
}

void ManagerPage::onAppInstall()
{
    qDebug() << __FUNCTION__;
//    if(shareData->updateStrMap.isEmpty())
//    {
//        return;
//    }

    for(int i = 0; i < installTable->rowCount(); i++)
    {
        ManagerWidget *managerWidget = (ManagerWidget *)installTable->cellWidget(i, 0);
        managerWidget->getButton(MANAGERBUTTON)->setStyleSheet("font-weight: 500; color:#027aff; border: 1px solid #027aff  ; border-radius: 2px; ");

        if(sender() == managerWidget->jsonFunc)
        {
            QString headUrl = managerWidget->shareData->updateStrMap.begin().value().proImage;
            QString appName = managerWidget->shareData->updateStrMap.begin().value().proName;
            QString packageName = managerWidget->shareData->updateStrMap.begin().value().packageName;
            QString appVersion = tr("Version : V") + managerWidget->shareData->updateStrMap.begin().value().version;
            double appSize = managerWidget->shareData->updateStrMap.begin().value().packageSize;
            QString pacSize = updm->transPackSize(appSize);
            QString appSizeStr = tr("Size : ") + pacSize;
            QString exefile = managerWidget->shareData->updateStrMap.begin().value().exeFile;

            managerWidget->setManagerButton(tr("Installing..."));
            managerWidget->setHeadUrl(headUrl);
            managerWidget->setAppName(appName);
            managerWidget->setPkgName(packageName);
            managerWidget->setVersion(appVersion);
            managerWidget->setExeFile(exefile);

            managerWidget->getImage(headUrl);
            managerWidget->getButton(NAMEBUTTON)->setText(appName);
            managerWidget->setVerLabel(appVersion);
            managerWidget->setSizeLabel(appSizeStr);

            QString flag = "INSTALLFLAG";
            int releaseId = managerWidget->getReleaseId();
            saveConfigFile(headUrl, appName, appVersion, appSizeStr, flag, exefile, releaseId, packageName);

            connect(managerWidget,SIGNAL(installSuccess(QString)),this,SLOT(onAppInstallSuccess(QString)));
            connect(managerWidget,SIGNAL(installFailure(QString)),this,SLOT(onAppInstallFailure(QString)));
//            connect(manTaskManager,SIGNAL(appInstallSuccess(QString)),this,SLOT(onAppInstallSuccess(QString)));
//            connect(manTaskManager,SIGNAL(appInstallFailure(QString)),this,SLOT(onAppInstallFailure(QString)));
            connect(managerWidget->getButton(MANAGERBUTTON),SIGNAL(clicked()),this,SLOT(insManBtnClicked()));

            managerWidget->installPackage(packageName);
            return;
        }
    }
}

void ManagerPage::onGetInsFinished(QMap<QString,INSTALLEDSTRUCT> insdMap)
{
    int insRowCount = insdMap.count();

    compTable->setRowCount(insRowCount);
    compTable->setMinimumHeight(96*insRowCount);

    QMap<QString,INSTALLEDSTRUCT>::iterator item;
    int count = 0;
    for(item = insdMap.begin(); item != insdMap.end(); item++)
    {
        //        qDebug() << "item.value().proName" << ;
        QString packageId = item.key();
//        QString appName = PackageKit::Transaction::packageName(packageId);   //appName
        QString appName = item.value().appName;
        QString appVersion = tr("Version : V") + PackageKit::Transaction::packageVersion(packageId);
        QString headUrl = item.value().proImage;
        QString exefile = item.value().exeFile;

        //        QString appVersion = item.value().
        ManagerWidget *manTaskManager = new ManagerWidget(this,headUrl,appName,appVersion,"");
        manTaskManager->setExeFile(exefile);
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
            managerWidget->getButton(UNINSBUTTON)->setText(tr("UnInstalling..."));
            QString packageID = managerWidget->getPkgId();
            QString appName = managerWidget->getAppName();
            emit sigUninstalling(appName,true,UNINSTALLING);
            managerWidget->removePackage(packageID);
            connect(managerWidget,SIGNAL(pacRmvSuccess(QString)),this,SLOT(deleteRmvRow(QString)));
            connect(managerWidget,SIGNAL(pacRmvFailure(QString)),this,SLOT(onRemoveFailure(QString)));

            break;
        }
        else if(sender() == managerWidget->getButton(MANAGERBUTTON))
        {
            qDebug() << "btn == managerButton:" << "row == " << i;
            QString openAppName =  managerWidget->getExeFile();
            QProcess *proc = new QProcess;
            proc->start(openAppName);
            break;
        }

    }
}

void ManagerPage::saveConfigFile(QString headUrl, QString appName, QString appVersion, QString appSizeStr, QString flag, QString exefile, int releaseId, QString packageName)
{
    QStringList appSizeList = appSizeStr.split(":");
qDebug() << "appSizeList == "<< appSizeList;
    QString appSize = appSizeList.at(1);

    QString relidStr = QString::number(releaseId);
    QSettings settings( "upins.ini", QSettings::IniFormat );
    settings.beginGroup(relidStr);
    settings.setValue("Url", headUrl);
    settings.setValue("Name", appName);
    settings.setValue("Version", appVersion);
    settings.setValue("Size", appSize);
    settings.setValue("Flag", flag);
    settings.setValue("Exe", exefile);
    settings.setValue("Relid", releaseId);
    settings.setValue("PkgName",packageName);
    settings.endGroup();
}

void ManagerPage::readConfigFile()
{
    QSettings settings( "upins.ini", QSettings::IniFormat );
    QStringList keys = settings.childGroups();
    qDebug() << "keys == " << keys;
    qDebug() << "keys.count() == " << keys.count();
    int saveCount = keys.count();
    QString section;
    QString headUrl;
    QString appName;
    QString appVersion;
    QString appSizeStr;
    QString flag;
    QString exefile;
    int releaseId;
    QString packageName;

    for(int i = 0; i < saveCount; i++)
    {
        section = keys.at(i);
        settings.beginGroup(section);
        headUrl = settings.value("Url").toString();
        appName = settings.value("Name").toString();
        appVersion = settings.value("Version").toString();
        appSizeStr = tr("Size : ") + settings.value("Size").toString();
        flag = settings.value("Flag").toString();
        exefile = settings.value("Exe").toString();
        releaseId = settings.value("Relid").toInt();
        packageName = settings.value("PkgName").toString();
        settings.endGroup();

        ManagerWidget *savedTaskManager = new ManagerWidget(this,headUrl, appName, appVersion, appSizeStr);
        savedTaskManager->getButton(MANAGERBUTTON)->setStyleSheet("font-weight: 500; color:#027aff; border: 1px solid #027aff  ; border-radius: 2px; ");

        savedTaskManager->setFlag(flag);
        savedTaskManager->setPkgName(packageName);
        savedTaskManager->setExeFile(exefile);
        savedTaskManager->setReleaseId(releaseId);
        if(flag == "INSTALLFLAG")
        {
            savedTaskManager->setManagerButton(tr("ReDownload"));
            connect(updm,SIGNAL(installSuccess()),savedTaskManager,SLOT(onInstallSuccess()));
            connect(updm,SIGNAL(installFailure()),savedTaskManager,SLOT(onInstallFailure()));
            connect(savedTaskManager,SIGNAL(appInstallSuccess(QString)),this,SLOT(onAppInstallSuccess(QString)));
            connect(savedTaskManager,SIGNAL(appInstallFailure(QString)),this,SLOT(onAppInstallFailure(QString)));
            connect(savedTaskManager->getButton(MANAGERBUTTON),SIGNAL(clicked()),this,SLOT(insManBtnClicked()));
        }
        else
        {
            savedTaskManager->setManagerButton(tr("ReUpdate"));
            connect(savedTaskManager->getButton(MANAGERBUTTON),SIGNAL(clicked()),this,SLOT(insManBtnClicked()));
        }
        savedTaskManager->getButton(UNINSBUTTON)->hide();
        installTable->insertRow(0);
        installTable->setCellWidget(0,0,savedTaskManager);
        int rowCount = installTable->rowCount();
        installTable->setMinimumHeight(96*rowCount);
    }
}

void ManagerPage::onStatusChanged()
{
    emit installStatusChanged();
}

void ManagerPage::sendSigReUpdate(QString appName)
{
    emit sigReUpdate(appName,true,REUPDATE);
}

void ManagerPage::sendSigReDownload(QString appName)
{
    emit sigReDownload(appName,true,REDOWNLOAD);
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


