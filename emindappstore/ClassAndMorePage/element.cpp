#include "element.h"
#include "qreplytimeout.h"
#include <QFont>
#include <QDebug>

Element::Element()
{
    baseWidget = new QWidget();
    baseWidget->setFixedSize(144,74);

    vbLayout = new QVBoxLayout();
    hbLayout = new QHBoxLayout();

    btnImage = new QPushButton();
    btnImage->setFixedSize(64,64);
    btnName = new QPushButton();
    btnName->setFixedSize(72,22);
    btnName->setStyleSheet("text-align: left;");//设置按钮文字显示位置-左对齐
    btnImage->setFlat(true);
    btnName->setFlat(true);
    vbLayout->addWidget(btnName);
    vbLayout->setMargin(0);
    vbLayout->setSpacing(0);

    hbLayout->addWidget(btnImage);
    btnStatus = new QPushButton();
    btnStatus->setFixedSize(72,24);
    //        btnStatus->setFlat(true);

    hbStartLayout = new QHBoxLayout();
    hbStartLayout->setContentsMargins(0,0,0,0);

    labelStar = new QLabel[5];
    for(int i=0;i<5;i++)
    {
        hbStartLayout->addWidget(&labelStar[i]);
    }

    vbLayout->addLayout(hbStartLayout);
    vbLayout->addWidget(btnStatus);
    connect(btnStatus,SIGNAL(clicked(bool)),this,SLOT(btnStatusSlot()));

    //去除矩形虚线框
    btnName->setFocusPolicy(Qt::NoFocus);
    btnImage->setFocusPolicy(Qt::NoFocus);
    btnStatus->setFocusPolicy(Qt::NoFocus);
    btnStatus->setCursor(Qt::PointingHandCursor);
    btnStatus->setObjectName("btnStatus");
    btnName->setCursor(Qt::PointingHandCursor);
    btnImage->setCursor(Qt::PointingHandCursor);
    hbLayout->addLayout(vbLayout);
    hbLayout->setMargin(0);
    hbLayout->setSpacing(0);
    baseWidget->setLayout(hbLayout);

    m_ImageManager = new QNetworkAccessManager();
    m_ImagePix = new QPixmap();
    m_Flag = 0;
    productId = 0;
    proStatus = 0;
    baseWidget->installEventFilter(this);
    connect(m_ImageManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)),Qt::QueuedConnection);
}

Element::~Element()
{

}

void Element::setBtnImage(QString imagePath)
{
    m_ImagePath = imagePath;
    m_Flag = 1;
}

void Element::setBtnName(QString name)
{
    //使文本生成省略号(...)
    QFontMetrics metrice(btnName->font());
    QString nameText = metrice.elidedText(name,Qt::ElideRight,btnName->width());
    btnName->setText(nameText);
    btnName->setToolTip(name);
}

void Element::setProStatus(int status)
{
    QString text;
    proStatus = status;
    if(status == 1)
    {
        text = "DownLoad";
    }
    else if(status == 2)
    {
        text = "Update";
    }
    else if(status == 3)
    {
        text = "open";
    }
    else if(status == 4)
    {
        text = "reDownLoad";
    }
    else if(status == 5)
    {
        text = "reUpdate";
    }
    else
    {
        qDebug("product status is error");
        text = "DownLoad";
    }
    btnStatus->setText(text);
}
void Element::updateProStatus(bool status, int flag)
{
    QString text;
    if(flag == 1 || flag == 2)
    {
        if(status)
        {
            if(proStatus == 1)
            {
                text = "Open";
                proStatus = 3;
            }
            else if(proStatus == 2)
            {
                text = "Open";
                proStatus = 3;
            }
            else if(proStatus == 4)
            {
                text = "Open";
                proStatus = 3;
            }
            else if(proStatus == 5)
            {
                text = "Open";
                proStatus = 3;
            }
            else
            {
                text = "DownLoad";
            }
        }
        else
        {
            if(proStatus == 1 || proStatus == 4)
            {
                text = "reDownLoad";
                proStatus = 4;
            }
            else if(proStatus == 2 || proStatus == 5)
            {
                text = "reUpdate";
                proStatus = 5;
            }
        }
    }
    else if(flag == 3)
    {
        text = "DownLoad";
        proStatus = 1;
    }
    btnStatus->setText(text);
    btnStatus->setEnabled(true);
}

void Element::setReleaseId(int id)
{
    releaseId = id;
}

//1-下载,2-更新,3-打开
void Element::btnStatusSlot()
{
    qDebug()<<__FUNCTION__<<" proStatus = "<<proStatus<<endl;
    if(proStatus == 1 || proStatus == 4)
    {
        btnStatus->setText("正在下载");
        btnStatus->setEnabled(false);
        emit installPackage(btnName->toolTip(),releaseId);
    }
    else if(proStatus == 2 || proStatus == 5)
    {
        btnStatus->setText("正在更新");
        btnStatus->setEnabled(false);
        emit updatePackage(btnName->toolTip(),releaseId);
    }
    else if(proStatus == 3)
    {
        process.start(exebleFile);
    }
    else
    {
        qDebug("product status is error!");
    }
}

void Element::setcategory(int cate)
{
    category = cate;
}

void Element::setProductId(int id)
{
    productId = id;
}

void Element::replyFinished(QNetworkReply *reply)
{
    QReplyTimeout *pTimeout = new QReplyTimeout(reply, 1000);
    // 超时进一步处理
    connect(pTimeout, &QReplyTimeout::timeout, [=]() {
        qDebug() << "Timeout";
    });

    if(reply->error() == QNetworkReply::NoError)
    {
        m_ImagePix->loadFromData(reply->readAll());
        btnImage->setIcon(*m_ImagePix);
        btnImage->setIconSize(btnImage->size());
    }
    else
    {
        qDebug()<<"reply is error!"<<endl;
    }

    reply->deleteLater();
}

void Element::initStar(double num)
{ 
    for(int i = 0;i < 5;i++)
    {
        labelStar[i].setFixedSize(10,10);
        labelStar[i].setPixmap(QPixmap(""));
    }

    for(int i = 0;i < (int)num;i++)
    {
        labelStar[i].setPixmap(QPixmap(":/image/refresh.png"));
    }

    if(num - (int)num > 0)
    {
        labelStar[(int)num].setPixmap(QPixmap(":/image/gift.png"));
    }
}

void Element::setExeFile(QString exe)
{
    exebleFile = exe;
}

QString Element::getBtnName()
{
    return btnName->toolTip();
}

bool Element::eventFilter(QObject *target, QEvent *event)
{
    if(target == baseWidget)
    {
        if(event->type() == QEvent::Paint)
        {
            if(m_Flag == 1)
            {
                m_Flag = 0;
                m_ImageManager->get(QNetworkRequest(QUrl(m_ImagePath)));
            }
        }
        return true;
    }
    return QWidget::eventFilter(target,event);
}


