#include "appwidget.h"
#include <QDebug>
#include <QPalette>
#include <QEvent>
#include <QNetworkRequest>
#include <QNetworkReply>

AppWidget::AppWidget(QWidget *parent, QString headUrl, QString nameStr, QString sizeStr, QString verStr, QString logStr, QString pkgStr) : QWidget(parent),changeLog(logStr),pkgId(pkgStr),iconUrl(headUrl),appName(nameStr),appVersion(verStr),appSize(sizeStr)
{
    appLayout = new QHBoxLayout();
    appLayout->setMargin(0);
    rightLayout = new QVBoxLayout();
    rightLayout->setMargin(0);
    bottomLayout = new QHBoxLayout();
    bottomLayout->setMargin(0);

    headButton = new QPushButton();
    headButton->setFlat(true);
    headButton->setStyleSheet("background: transparent;");
    headButton->setFixedSize(64,64);
    getImage(headUrl);

    nameButton = new QPushButton();
    nameButton->setFlat(true);
    nameButton->setMaximumWidth(120);
    nameButton->setStyleSheet("color:#313131; font-weight: 500; font-size:14px; text-align: left; background: transparent;");
    setBtnMetric(nameStr,nameButton);

    QStringList logList = logStr.split("#");
    introstr = logList.at(0);
    QString newfuncstr = logList.at(1);
    QStringList newfuncList = newfuncstr.split("*");
    int count = newfuncList.count();
    for(int i = 0; i < count; i++)
    {
        if(i != (count -1))
        {
            newFuncStr += "- " + newfuncList.at(i) + "\n";
        }
        else
        {
            newFuncStr += "- " + newfuncList.at(i);
        }
    }
    introLabel = new QLabel();
    introLabel->setFrameStyle(QFrame::NoFrame);
    introLabel->setStyleSheet("color:#969696;");
    introLabel->setText(introstr);

    segLabel = new QLabel();
    segLabel->setStyleSheet("background:#cccccc");
    segLabel->setFixedSize(1,15);

    funcButton = new QPushButton();
    funcButton->setFlat(true);
    funcButton->setText(tr("New Function"));
    funcButton->setFlat(true);
    QPalette fucpal;
    fucpal.setColor(QPalette::ButtonText, QColor(2,122,255));
    funcButton->setPalette(fucpal);
    funcButton->setStyleSheet("text-align: left;");
    funcButton->setMaximumWidth(90);

    sizeLabel = new QLabel();
    sizeLabel->resize(16,64);
    sizeLabel->setText(sizeStr);

    versionLabel = new QLabel();
    versionLabel->setText(verStr);

    updateButton = new QPushButton();
    updateButton->setFixedSize(80,32);
    updateButton->setStyleSheet("font-weight: 500; color:#027aff; border: 1px solid #027aff  ; border-radius: 2px; ");
    updateButton->setText(tr("Update"));

    appLayout->addWidget(headButton);
    appLayout->setSpacing(16);
    appLayout->addLayout(rightLayout);
    rightLayout->addStretch(24);
    rightLayout->addWidget(nameButton);
    rightLayout->setSpacing(3);
    rightLayout->addLayout(bottomLayout);
    rightLayout->addStretch(24);
    bottomLayout->addWidget(introLabel);
    bottomLayout->addSpacing(5);
    bottomLayout->addWidget(segLabel);
    bottomLayout->addSpacing(5);
    bottomLayout->addWidget(funcButton);
    bottomLayout->addStretch();
    appLayout->addWidget(sizeLabel);
    appLayout->addSpacing(112);
    appLayout->addWidget(versionLabel);
    appLayout->addSpacing(112);
    appLayout->addWidget(updateButton);
    setLayout(appLayout);
}

void AppWidget::getImage(QString headUrl)
{
    imageUpdate = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(QUrl(headUrl));
    connect(imageUpdate, SIGNAL(finished(QNetworkReply *)), this, SLOT(getImageFinished(QNetworkReply *)));
    imageUpdate->get(request);
}

void AppWidget::getImageFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        QPixmap pixmap;
        pixmap.loadFromData(bytes);
        headButton->setIcon(pixmap);
        headButton->setIconSize(QSize(64,64));
    }
}

void AppWidget::installUpdate(const QString &packageId)
{
//    qDebug() << __FUNCTION__ << "packageId == " << packageId;

    PackageKit::Transaction::TransactionFlag flag = PackageKit::Transaction::TransactionFlagOnlyTrusted;
    m_updatesTrans = PackageKit::Daemon::updatePackage(packageId, flag);

//    connect(m_updatesTrans.data(), &PackageKit::Transaction::statusChanged, this, &PkUpdates::onStatusChanged);
    connect(m_updatesTrans.data(), &PackageKit::Transaction::package, this, &AppWidget::onPackageUpdating);
    connect(m_updatesTrans.data(), &PackageKit::Transaction::finished, this, &AppWidget::onUpdateFinished);

}

void AppWidget::onPackageUpdating(PackageKit::Transaction::Info info, const QString &packageID)
{
    Q_UNUSED(info);
    const uint percent = m_updatesTrans->percentage();
    qDebug() << "Package updating:" << packageID << "percent == " << percent;
}

void AppWidget::onUpdateFinished(PackageKit::Transaction::Exit status, uint runtime)
{
    Q_UNUSED(runtime);
    if (status == PackageKit::Transaction::ExitSuccess)
    {
        emit updateOk();
        return;
    }
    else
    {
        emit updateFailure();
        qDebug() << "Update package didn't finish successfully";
    }

}

void AppWidget::setBtnMetric(QString btnText,QPushButton *pushButton)
{
    QFontMetrics metric(btnText);
    QString nameStr = metric.elidedText(btnText,Qt::ElideRight,pushButton->width());
    pushButton->setText(nameStr);
    pushButton->setToolTip(btnText);
}

bool AppWidget::event(QEvent *event)
{
    if(event->type() == QEvent::Resize )
    {
        emit sigIntroResize();
        return true;
    }
    return QWidget::event(event);
}

QPushButton* AppWidget::getHeadButton()
{
    return headButton;
}

QPushButton* AppWidget::getNameButton()
{
    return nameButton;
}

QPushButton* AppWidget::getFuncButton()
{
    return funcButton;
}

QPushButton* AppWidget::getUpdateButton()
{
    return updateButton;
}

QString AppWidget::getChangeLog()
{
    return changeLog;
}

QLabel *AppWidget::getIntroLabel()
{
    return introLabel;
}

QString AppWidget::getIntroStr()
{
    return introstr;
}

QString AppWidget::getNewFunStr()
{
    return newFuncStr;
}

QString AppWidget::getPkgId()
{
    return pkgId;
}
QString AppWidget::getHeadUrl()
{
    return iconUrl;
}

QString AppWidget::getAppName()
{
    return appName;
}

QString AppWidget::getAppVer()
{
    return appVersion;
}

QString AppWidget::getAppSize()
{
    return appSize;
}

QString AppWidget::getExeFile()
{
    return exeFile;
}

QString AppWidget::getProductId()
{
    return productId;
}

void AppWidget::setProductId(int productid)
{
    productId = productid;
}

void AppWidget::setExeFile(QString exefile)
{
    exeFile = exefile;
}

int AppWidget::getReleaseId()
{
    return releaseId;
}

void AppWidget::setReleaseId(int relid)
{
    releaseId = relid;
}

QString AppWidget::getPkgName()
{
    return pkgName;
}

void AppWidget::setPkgName(QString pkgname)
{
    pkgName = pkgname;
}

