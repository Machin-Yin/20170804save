#include <QHeaderView>
#include <QDebug>
#include <QLoggingCategory>
#include "updatepage.h"

UpdatePage::UpdatePage(QWidget *parent) : QWidget(parent)
{
    upd = new PkUpdates(this);
//    upd->checkUpdates();
    upd->getUpdateData();
    createUpdateWindow();
}

void UpdatePage::createUpdateWindow()
{
    upScroArea = new QScrollArea(this);
    upScroArea->resize(960,640);
    upScroArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    upScroArea->setFrameShape(QFrame::NoFrame);
    pageUpdateWidget = new QWidget();

    updateTable = new QTableWidget(this);
    //    updateTable->setBaseSize(960,640);
    updateTable->setShowGrid(false);
    updateTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    updateTable->setFrameShape(QFrame::NoFrame);
    updateTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    updateTable->setFocusPolicy(Qt::NoFocus);

    updTaskBar = new TaskBar(this,"0个应用可升级","全部更新");
    connect(upd,SIGNAL(sigUpdateData(UPDATESTRUCTMAP)),this,SLOT(onGetupFinished(UPDATESTRUCTMAP)));
    connect(updTaskBar->getOnekeyBtn(),SIGNAL(clicked()),this,SLOT(oneKeyBtnclicked()));

    upVLayout = new QVBoxLayout();
    upVLayout->setMargin(16);
    upVLayout->addWidget(updTaskBar);
    upVLayout->addWidget(updateTable);
    this->setLayout(upVLayout);

    pageUpdateWidget->setLayout(upVLayout);
    upScroArea->setWidget(pageUpdateWidget);
    upScroArea->setWidgetResizable(true);

}

void UpdatePage::onGetupFinished(UPDATESTRUCTMAP updateMap)
{
//    qDebug() << endl << __FUNCTION__ ;
    int rowCount = updateMap.count();
    updTaskBar->setTaskLabel(rowCount);
    updateTable->setColumnCount(1);
    updateTable->setRowCount(rowCount);

    //    updateTable->resizeColumnToContents(0);
    updateTable->setStyleSheet(
                "QTableWidget {"
                "background-color: #00000000;"
                "}"
                "QTableWidget::item {"
                "border-top: 1px solid #dcdcdc;"
                "}"
                "QTableWidget::item:selected{"
                "background: white;"
                "}"
                );
    updateTable->verticalHeader()->setDefaultSectionSize(96);
    updateTable->horizontalHeader()->setStretchLastSection(true);
    updateTable->setFrameStyle(QFrame::NoFrame);

    updateTable->setMinimumHeight(96*rowCount);

    QHeaderView* vheaderView = updateTable->verticalHeader();
    vheaderView->setHidden(true);
    QHeaderView *hheaderView = updateTable->horizontalHeader();
    hheaderView->setHidden(true);

    QMap<int,UPDATESTRUCT>::iterator itor;
    int count = 0;
    for(itor = updateMap.begin(); itor != updateMap.end(); itor++)
    {
        QString headUrl = itor.value().proImage;
        QString appName = itor.value().proName;
        QString changeLog = itor.value().changeLog;
        int psize = itor.value().packageSize;
        QString packSize = transPackSize(psize);
        QString packVersion = itor.value().version;

        QVariantMap updateList = upd->packages();
        QString pkgId;
        QVariantMap::iterator iter;
        for(iter = updateList.begin(); iter != updateList.end(); iter++)
        {
            QString packageName = PackageKit::Transaction::packageName(iter.key());
            if(appName == packageName)
            {
                pkgId = iter.key();
                break;
            }
        }

        AppWidget* appWidget = new AppWidget(this,headUrl,appName,packSize,packVersion,changeLog,pkgId);
        updateTable->setCellWidget(count,0,appWidget);
        count++;

        connect(appWidget->getHeadButton(),SIGNAL(clicked()),this,SLOT(pageUpdateBtnClicked()));
        connect(appWidget->getNameButton(),SIGNAL(clicked()),this,SLOT(pageUpdateBtnClicked()));
        connect(appWidget->getFuncButton(),SIGNAL(clicked()),this,SLOT(pageUpdateBtnClicked()));
        connect(appWidget->getUpdateButton(),SIGNAL(clicked()),this,SLOT(pageUpdateBtnClicked()));
        connect(appWidget,SIGNAL(sigIntroResize()),this,SLOT(strLenChanged()));
    }
    updateTable->setRowHeight(0,95);
    updateTable->setRowHeight(0,96);
}

void UpdatePage::pageUpdateBtnClicked()
{
    for(int i = 0; i < updateTable->rowCount(); i++)
    {
        AppWidget* appWidget = (AppWidget*)updateTable->cellWidget(i,0);

        if(sender() == appWidget->getHeadButton() )
        {
            //            qDebug() << "btn == headButton:" << "row == " << i;
            break;
        }
        else if(sender() == appWidget->getNameButton())
        {
            //            qDebug() << "btn == nameButton:" << "row == " << i;
            break;
        }
        else if(sender() == appWidget->getFuncButton())
        {
            int tableHeight = updateTable->height();

            updateTable->insertRow(i+1);
            QString funcStr = appWidget->getChangeLog().split("#").at(1);
            FuncWidget *nfuncWidget = new FuncWidget(this,funcStr);
            updateTable->setCellWidget(i+1,0,nfuncWidget);
            int textHeight  = nfuncWidget->getNfuncEdit()->document()->size().height();
            int rowHeight = textHeight+20+16+18+22;
            updateTable->setRowHeight(i+1,rowHeight);
            appWidget->getFuncButton()->setEnabled(false);
            updateTable->setMinimumHeight(tableHeight + rowHeight);

            connect(nfuncWidget->getHideButton(),SIGNAL(clicked()),this,SLOT(pageUpdateBtnClicked()));
            connect(nfuncWidget,SIGNAL(sigTextHeight(int)),this,SLOT(textAreaChanged(int)));

            break;
        }
        else if(sender() == appWidget->getUpdateButton())
        {
            qDebug() << "btn == updateButton:" << "row == " << i;

            QString iconUrl = appWidget->getHeadUrl();
            QString appName = appWidget->getAppName();
            QString appVersion = tr("Version：") + appWidget->getAppVer();
            QString appSize = tr("Size：") + appWidget->getAppSize();
            emit theUpdateApp(iconUrl, appName, appVersion, appSize);

            QString pkgId = appWidget->getPkgId();
            appWidget->getUpdateButton()->setText("Updating...");
            connect(upd,SIGNAL(updateOk()),appWidget,SLOT(onUpdateOk()));
            connect(appWidget,SIGNAL(appUpdateFinished()),this,SLOT(onAppUpdateFinished()));
            upd->installUpdate(pkgId);
            break;
        }
        else
        {
            FuncWidget* gfuncWidget = (FuncWidget*)updateTable->cellWidget(i,0);
            if(sender() == gfuncWidget->getHideButton())
            {
                //                qDebug() << "btn == hideButton:" << "row == " << i;
                int rmvTableHeight = updateTable->height();
                int rmvRowHeight = updateTable->rowHeight(i);
                int tableHeight = rmvTableHeight - rmvRowHeight;
                AppWidget *eblBtnWid = (AppWidget *)updateTable->cellWidget(i-1,0);
                eblBtnWid->getFuncButton()->setEnabled(true);
                updateTable->removeRow(i);
                updateTable->setMinimumHeight(tableHeight);
            }
        }
    }
}

void UpdatePage::oneKeyBtnclicked()
{
    int updCount = updateTable->rowCount();
    for(int i = 0; i < updCount; i++)
    {
        AppWidget* appWidget = (AppWidget*)updateTable->cellWidget(i,0);

        QString iconUrl = appWidget->getHeadUrl();
        QString appName = appWidget->getAppName();
        QString appVersion = tr("Version：") + appWidget->getAppVer();
        QString appSize = tr("Size：") + appWidget->getAppSize();
        emit theUpdateApp(iconUrl, appName, appVersion, appSize);

        QString pkgId = appWidget->getPkgId();
        appWidget->getUpdateButton()->setText("Updating...");
        connect(upd,SIGNAL(updateOk()),appWidget,SLOT(onUpdateOk()));
        connect(appWidget,SIGNAL(appUpdateFinished()),this,SLOT(onAppUpdateFinished()));
        upd->installUpdate(pkgId);

        if(!appWidget->getFuncButton()->isEnabled())
            i++;

    }
}

void UpdatePage::onAppUpdateFinished()
{
    for(int i = 0; i < updateTable->rowCount(); i++)
    {
        AppWidget* appWidget = (AppWidget*)updateTable->cellWidget(i,0);
        if(sender() == appWidget)
        {
            qDebug() << "remove row :" << i;
            emit appUpdateOk(appWidget->getAppName() ,appWidget->getHeadUrl(), appWidget->getAppVer());
            if(!appWidget->getFuncButton()->isEnabled())
                updateTable->removeRow(i+1);
            updateTable->removeRow(i);
            int count = updateTable->rowCount();
            updateTable->setMinimumHeight(96*count);
            updTaskBar->setTaskLabel(count);
        }
    }
}

void UpdatePage::strLenChanged()
{
    for(int i = 0; i < updateTable->rowCount(); i++)
    {
        AppWidget* chLabelWidth = (AppWidget*)updateTable->cellWidget(i,0);
        if(sender() == chLabelWidth)
        {
            chLabelWidth->getIntroLabel()->setText(chLabelWidth->getIntroStr());
            int labelWidth = chLabelWidth->getIntroLabel()->width();
            QFont font;
            QFontMetrics fm(font);
            int fontSize = fm.width(chLabelWidth->getIntroStr());
            QString fmstr = chLabelWidth->getIntroStr();
            labelWidth = updateTable->width() - 520;
            if(labelWidth > 550)
            {
                labelWidth = 550;
            }
            if(fontSize > labelWidth)
            {
                fmstr = fm.elidedText(chLabelWidth->getIntroStr(), Qt::ElideRight, labelWidth-130);
            }
            chLabelWidth->getIntroLabel()->setText(fmstr);
        }
    }
}

void UpdatePage::textAreaChanged(int hig)
{
    for(int i = 0; i < updateTable->rowCount(); i++)
    {
        FuncWidget* chEditHeight = (FuncWidget*)updateTable->cellWidget(i,0);
        if(sender() == chEditHeight)
        {
            int rowHeight = hig+20+16+18+22;
            updateTable->setRowHeight(i,rowHeight+5);
        }
    }
}

QString UpdatePage::transPackSize(const double &size)
{
    double packSize = size;
    static QStringList measures;
    if (measures.isEmpty())
        measures << QCoreApplication::translate("QInstaller", "B")
                 << QCoreApplication::translate("QInstaller", "K")
                 << QCoreApplication::translate("QInstaller", "M")
                 << QCoreApplication::translate("QInstaller", "G")
                 << QCoreApplication::translate("QInstaller", "T");
    QStringListIterator it(measures);
    QString measure(it.next());
    while (packSize >= 1024.0 && it.hasNext()) {
        measure = it.next();
        packSize /= 1024.0;
    }
    return QString::fromLatin1("%1%2").arg(packSize, 0, 'f', 1).arg(measure);
}


bool UpdatePage::event(QEvent *event)
{
    if(event->type() == QEvent::Resize)
    {
        upScroArea->resize(QSize(this->width(),this->height()));
        return true;
    }
    return QWidget::event(event);
}

