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
    btnStatus = new CustomButton();
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
    setBtnStatus("download");
    connect(btnStatus,SIGNAL(clicked(bool)),this,SLOT(btnStatusSlot()));

    //去除矩形虚线框
    btnName->setFocusPolicy(Qt::NoFocus);
    btnImage->setFocusPolicy(Qt::NoFocus);
    btnStatus->setFocusPolicy(Qt::NoFocus);
    btnStatus->setCursor(Qt::PointingHandCursor);
    btnStatus->setStyleSheet("border:1px groove;border-radius:2px;border-color:#c8c8c8");
    btnName->setCursor(Qt::PointingHandCursor);
    btnImage->setCursor(Qt::PointingHandCursor);
    hbLayout->addLayout(vbLayout);
    hbLayout->setMargin(0);
    hbLayout->setSpacing(0);
    baseWidget->setLayout(hbLayout);

    m_ImageManager = new QNetworkAccessManager();
    m_ImagePix = new QPixmap();
    m_Flag = 0;
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

    //    QPixmap pix = QPixmap(imagePath);
    //    btnImage->setIcon(pix);
    //    btnImage->setIconSize(pix.size());
    ////    btnImage->setStyleSheet(imagePath);
}

void Element::setBtnName(QString name)
{
    //使文本生成省略号(...)
    QFontMetrics metrice(btnName->font());
    QString nameText = metrice.elidedText(name,Qt::ElideRight,btnName->width());
    btnName->setText(nameText);
    btnName->setToolTip(name);
}

void Element::setBtnStart()
{

}

void Element::setBtnStatus(QString status)
{
    btnStatus->setText(status);
    btnStatus->setToolTip(status);
}

void Element::setButton(bool)
{

}

void Element::btnStatusSlot()
{
    btnStatus->setText("NULL");
    btnStatus->setEnabled(false);
    btnStatus->setToolTip(btnStatus->text());
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
        labelStar[i].setPixmap(QPixmap(":/image/gift.png"));
    }

    if(num - (int)num > 0)
    {
        labelStar[(int)num].setPixmap(QPixmap(":/image/gift.png"));
    }
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


