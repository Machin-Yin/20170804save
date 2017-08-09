#include "appwidget.h"
#include <QDebug>
#include <QPalette>
#include <QEvent>
#include <QNetworkRequest>
#include <QNetworkReply>

//#define SIZE_W 250

AppWidget::AppWidget(QWidget *parent, QString headUrl, QString nameStr, QString sizeStr, QString verStr, QString logStr, QString pkgStr) : QWidget(parent),changeLog(logStr),pkgId(pkgStr),iconUrl(headUrl),appName(nameStr),appVersion(verStr),appSize(sizeStr)
{
    appLayout = new QHBoxLayout();
    appLayout->setMargin(0);
    rightLayout = new QVBoxLayout();
    rightLayout->setMargin(0);
    bottomLayout = new QHBoxLayout();

    headButton = new QPushButton();
    headButton->setFlat(true);
    getImage(headUrl);

    nameButton = new QPushButton();
    nameButton->setFlat(true);
    nameButton->setMaximumWidth(180);
    nameButton->setStyleSheet("text-align: left;");
    nameButton->setText(nameStr);

    QStringList logList = logStr.split("#");
    introstr = logList.at(0);
    introLabel = new QLabel();

    introLabel->setStyleSheet("border-right: 1px; border-style: solid; border-color: #cccccc;");
    introLabel->setText(introstr);

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
//    QPalette uppal;
//    uppal.setColor(QPalette::ButtonText, QColor(2,122,255));
//    updateButton->setPalette(uppal);
//    updateButton->setStyleSheet("border: 1px solid #027aff  ; border-radius: 2px; ");
//    updateButton->setStyleSheet("border-color: #027aff; border-width: 1px; border-style: solid; border-radius: 2px; ");
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

void AppWidget::onUpdateOk()
{
    emit appUpdateFinished();
}

void AppWidget::onUpdateFailure()
{
    qDebug() << __FUNCTION__;
    emit appUpdateFailure();
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

