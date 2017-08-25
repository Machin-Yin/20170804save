#include "detailwidget.h"
#include <QQuickWidget>
#include <QLabel>
#include <QPushButton>
#include <QQuickWidget>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QQmlEngine>
#include <QProgressBar>
#include <QGridLayout>
#include <QScrollArea>
#include "sharedata.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "jsonfunc.h"
#include <QCoreApplication>
#include <QtMath>

DetailWidget::DetailWidget(QWidget *parent,ShareData *sdata,JSONFUNC   *jsonData) : QWidget(parent)
{
    shareData = sdata;
    jsonFunc = jsonData;

    connect(jsonFunc,SIGNAL(updateIsOk()),this,SLOT(updateJsonOk()));
    initialWidgets();
    layoutWidgets();

}

DetailWidget::~DetailWidget()
{
    //    delete jsonFunc;
    //    delete shareData;
    delete[] lbCommentStar;
    delete[] lbUserCommStar;
    delete[] btnMyStarLevel;
}

void DetailWidget::initialWidgets()
{
    lbProductImg = new QLabel(this);
    lbProductImg->resize(128,128);

    lbProductName = new QLabel(this);

    lbComment = new QLabel(tr("Comment:"),this);
    lbCommentStar = new QLabel[5];
    lbCommentMark = new QLabel(this);
    lbProductVersion = new QLabel(tr("Version:"),this);
    lbVersionNo = new QLabel(this);
    lbProductSize = new QLabel(tr("Size:"),this);
    lbSizeValue = new QLabel(this);
    btnState = new QPushButton(this);
    connect(btnState,SIGNAL(clicked()),this,SLOT(onBtnStateClicked()));

    lbAppInfo = new QLabel(this);
    lbAppInfo->setText(tr("Application Introduction"));
    lbAppInfoDetail = new QLabel(this);
    lbUserComment = new QLabel(this);
    lbUserCommMark = new QLabel(this);
    lbUserCommMark->setObjectName("lbUserCommMark");
    lbUserCommStar = new QLabel[5];
    for(int i=0;i<5;i++)
        lbUserCommStar[i].resize(16,16);
    lbUserCommStar[0].setPixmap(QPixmap("qrc:/image/Tux.png"));
    lbUserCommStar[1].setPixmap(QPixmap("qrc:/image/Tux.png"));
    lbUserCommStar[2].setPixmap(QPixmap("qrc:/image/Tux.png"));
    lbUserCommStar[3].setPixmap(QPixmap("qrc:/image/Tux.png"));
    lbUserCommStar[4].setPixmap(QPixmap("qrc:/image/Tux.png"));

    lbTotalCommenters = new QLabel(this);
    lbStarLevel1 = new QLabel(this);
    lbStarLevel2 = new QLabel(this);
    lbStarLevel3 = new QLabel(this);
    lbStarLevel4 = new QLabel(this);
    lbStarLevel5 = new QLabel(this);
    pbStarLevel1 = new QProgressBar(this);
    pbStarLevel2 = new QProgressBar(this);
    pbStarLevel3 = new QProgressBar(this);
    pbStarLevel4 = new QProgressBar(this);
    pbStarLevel5 = new QProgressBar(this);
    lbLevel1Users = new QLabel(this);
    lbLevel2Users = new QLabel(this);
    lbLevel3Users = new QLabel(this);
    lbLevel4Users = new QLabel(this);
    lbLevel5Users = new QLabel(this);

    lbMyMark = new QLabel(this);
    btnMyStarLevel = new QPushButton[5];
    for(int i=0;i<5;i++)
        btnMyStarLevel[i].resize(24,24);
    btnMyStarLevel[0].setIcon(QPixmap("qrc:/image/Tux.png"));
    btnMyStarLevel[1].setIcon(QPixmap("qrc:/image/Tux.png"));
    btnMyStarLevel[2].setIcon(QPixmap("qrc:/image/Tux.png"));
    btnMyStarLevel[3].setIcon(QPixmap("qrc:/image/Tux.png"));
    btnMyStarLevel[4].setIcon(QPixmap("qrc:/image/Tux.png"));

    //    lbmyStarLevel = new QLabel(this);
    lbmyComment = new QLabel(this);
    teCommContent = new QTextEdit(this);
    teCommContent->setMinimumHeight(200);
    teCommContent->setMaximumHeight(200);
    btnSubmit = new QPushButton(tr("submit"),this);
    btnCancel = new QPushButton(tr("Cancel"),this);

    qmlEnginer = new QQmlEngine;
    imgBoxer = new QQuickWidget(qmlEnginer,this);
    userCommBoxer = new QQuickWidget(qmlEnginer,this);

    imgBoxer->setSource(QUrl::fromLocalFile(":/Imgboxer.qml"));
    imgBoxer->setMinimumHeight(320);


    userCommBoxer->setSource(QUrl::fromLocalFile(":/Comment.qml"));
    userCommBoxer->setMinimumHeight(600);




    hly1 = new QHBoxLayout;
    hly2 = new QHBoxLayout;
    hly3 = new QHBoxLayout;
    hly4 = new QHBoxLayout;
    hly5 = new QHBoxLayout;
    hly6 = new QHBoxLayout;
    hly7 = new QHBoxLayout;
    hly8 = new QHBoxLayout;
    hly9 = new QHBoxLayout;
    hly10 = new QHBoxLayout;
    //    hly11 = new QHBoxLayout;
    //    hly12 = new QHBoxLayout;
    vly1 = new QVBoxLayout;
    vly2 = new QVBoxLayout;
    vly3 = new QVBoxLayout;
    vly4 = new QVBoxLayout;
    vly5 = new QVBoxLayout;
    //    vly6 = new QVBoxLayout;
    vly = new QVBoxLayout;
    gly = new QGridLayout;

}

void DetailWidget::refreshPage(int)
{


}

void DetailWidget::layoutWidgets()
{
    //part1

    hly2->addWidget(lbComment);

    hly2->addWidget(lbCommentStar);
    hly2->addWidget(lbCommentStar+1);
    hly2->addWidget(lbCommentStar+2);
    hly2->addWidget(lbCommentStar+3);
    hly2->addWidget(lbCommentStar+4);

    hly2->addWidget(lbCommentMark);
    hly3->addWidget(lbProductVersion);
    hly3->addWidget(lbVersionNo);
    hly4->addWidget(lbProductSize);
    hly4->addWidget(lbSizeValue);


    vly1->addWidget(lbProductName);
    vly1->addLayout(hly2);
    vly1->addLayout(hly3);
    vly1->addLayout(hly4);

    lbProductName->setText("qq Music");
    lbProductImg->setText("nice");
    hly1->addWidget(lbProductImg);
    hly1->addLayout(vly1);
    hly1->setAlignment(lbProductImg,Qt::AlignLeft);
    hly1->setAlignment(vly1,Qt::AlignRight);

    hly5->addLayout(hly1);
    hly5->addWidget(btnState);
    hly5->setAlignment(hly1,Qt::AlignLeft);
    hly5->setAlignment(btnState,Qt::AlignRight);

    //part2
    vly2->addWidget(lbAppInfo);
    vly2->addWidget(lbAppInfoDetail);

    //part3
    vly3->addWidget(lbUserComment);
    lbUserComment->setText(tr("User Comment Mark"));
    vly4->addWidget(lbUserCommMark);
    hly10->addWidget(&lbUserCommStar[0]);
    hly10->addWidget(&lbUserCommStar[1]);
    hly10->addWidget(&lbUserCommStar[2]);
    hly10->addWidget(&lbUserCommStar[3]);
    hly10->addWidget(&lbUserCommStar[4]);
    vly4->addLayout(hly10);
    vly4->addWidget(lbTotalCommenters);
    lbTotalCommenters->setText("152525 people");


    gly->addWidget(lbStarLevel1,0,0);
    gly->addWidget(pbStarLevel1,0,1);
    gly->addWidget(lbLevel1Users,0,2);

    gly->addWidget(lbStarLevel2,1,0);
    gly->addWidget(pbStarLevel2,1,1);
    gly->addWidget(lbLevel2Users,1,2);

    gly->addWidget(lbStarLevel3,2,0);
    gly->addWidget(pbStarLevel3,2,1);
    gly->addWidget(lbLevel3Users,2,2);

    gly->addWidget(lbStarLevel4,3,0);
    gly->addWidget(pbStarLevel4,3,1);
    gly->addWidget(lbLevel4Users,3,2);

    gly->addWidget(lbStarLevel5,4,0);
    gly->addWidget(pbStarLevel5,4,1);
    gly->addWidget(lbLevel5Users,4,2);


    hly7->addWidget(lbMyMark);
    lbMyMark->setText(tr("My Comment:"));
    hly7->addWidget(&btnMyStarLevel[0]);
    hly7->addWidget(&btnMyStarLevel[1]);
    hly7->addWidget(&btnMyStarLevel[2]);
    hly7->addWidget(&btnMyStarLevel[3]);
    hly7->addWidget(&btnMyStarLevel[4]);
    hly7->addWidget(lbmyComment);
    lbmyComment->setText(tr("Good"));

    hly9->addLayout(vly4);
    hly9->addLayout(gly);
    hly9->addLayout(hly7);

    //input comment area
    vly5->addWidget(teCommContent);
    hly8->addWidget(btnSubmit);
    hly8->addWidget(btnCancel);
    vly5->addLayout(hly8);
    vly5->setAlignment(hly8,Qt::AlignRight);

    vly3->addLayout(hly9);
    vly3->addLayout(vly5);

    //
    vly->addLayout(hly5);
    vly->addWidget(imgBoxer);
    vly->addLayout(vly2);
    vly->addLayout(vly3);
    vly->addWidget(userCommBoxer);

    spItem = new QSpacerItem(100,200,QSizePolicy::Expanding,QSizePolicy::Expanding);
    vly->insertSpacerItem(-1,spItem);


//    vly->setContentsMargins(16,0,16,0);
    vly->setMargin(32);
    setLayout(vly);
}

//from mainwindow to this page,show the data
void DetailWidget::toPage(int productId)
{
    int releaseAry[1] = {0};
    //    int productIds[1] = {0};
    mProductId = productId;

    for(auto item = shareData->classStrMap.begin();item != shareData->classStrMap.end();item++)
    {

        if(item.key() == productId ){
            shareData->updateStrMap.clear();
            releaseAry[0] = item.value().releaseId;
            //get updateStrMap
            jsonFunc->getUpdateRelease(releaseAry,1);
            getImage(item.value().proImage);
            lbProductName->setText(item.value().proName);
            lbCommentMark->setText(QString::number(item.value().proStar,'f',1)+tr("fen"));
            //get User Comments
            //            jsonFunc->getComment(releaseAry);
            lbUserCommMark->setText(QString::number(item.value().proStar,'f',1));




            //1:下载 2:更新 3:卸载 4:重新下载 5:重新更新 6:Open
            switch(item.value().proStatus){
            case 1:
                btnState->setText(tr("Download"));
                break;
            case 2:
                btnState->setText(tr("Update"));
                break;
            case 3:
                btnState->setText(tr("Uninstall"));
                break;
            case 4:
                btnState->setText(tr("ReDownload"));
                break;
            case 5:
                btnState->setText(tr("ReUpdate"));
                break;
            case 6:
                btnState->setText(tr("Open"));
                break;
            default:
                break;
            }

            lbAppInfoDetail->setText(item.value().proDescription);
            break;
        }


    }

}

// get updateStrMap data and show the packagesize
bool DetailWidget::updateJsonOk()
{
    auto item1 = shareData->updateStrMap.begin();

    if(item1.value().productId == mProductId)
    {
        lbSizeValue->setText(octetsToString(static_cast<qint64>(item1.value().packageSize)));
        lbVersionNo->setText( "V" + item1.value().version );
    }


    return true;
}


void DetailWidget::getImage(QString headUrl)
{
    imageManager = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(QUrl(headUrl));
    connect(imageManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));
    imageManager->get(request);
}

void DetailWidget::replyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        QPixmap pixmap;
        pixmap.loadFromData(bytes);
        lbProductImg->setPixmap(pixmap);
        lbProductImg->resize(128,128);

    }
}

void DetailWidget::initStar(double num)
{
    for(int i = 0;i < 5;i++)
    {
        lbCommentStar[i].setFixedSize(10,10);
        lbCommentStar[i].setPixmap(QPixmap(":/image/空星.png"));
    }

    for(int i = 0;i < num;i++)
    {
        lbCommentStar[i].setPixmap(QPixmap(":/image/全星.png"));
    }

    if(num - qFloor(num) > 0)
    {
        lbCommentStar[(int)num].setPixmap(QPixmap(":/image/半星.png"));
    }
}

QString DetailWidget::octetsToString( qint64 octets )
{
#define THE_FACTOR 1024
    static const qint64 kb = THE_FACTOR;
    static const qint64 mb = THE_FACTOR * kb;
    static const qint64 gb = THE_FACTOR * mb;

    QString s;
    qreal value = octets;

    // Whether we care about decimals: only for GB and only
    // if it's less than 10 GB.
    bool round = true;

    if (octets >= gb) {
        s = QCoreApplication::translate("Size", "%L1 GB");
        value /= gb;
        round = false;
    } else if (octets >= mb) {
        s = QCoreApplication::translate("Size", "%L1 MB");
        value /= mb;
    } else if (octets >= kb) {
        s = QCoreApplication::translate("Size", "%L1 KB");
        value /= kb;
    } else  {
        s = QCoreApplication::translate("Size", "%L1 B");
    }

    if (value > 9.95)
        round = true;

    if (round)
        return s.arg(qRound(value));

    return s.arg(value, 0, 'f', 2);
}

//1:下载 2:更新 3:卸载 4:重新下载 5:重新更新 6:open
void DetailWidget::updatePackageStatus(QString productName, bool bo,int flag)
{
    qDebug()<<__FUNCTION__<<endl;
    for(auto item = shareData->classStrMap.begin();item.value().proName != productName ; item++ )
    {
        switch(flag){
        case 1:
            if(bo){
                btnState->setText(tr("Open"));
            }else{
                btnState->setText(tr("ReDownload"));
            }
            break;

        case 2:
            if(bo){
                btnState->setText(tr("Open"));
            }else{
                btnState->setText(tr("ReUpdate"));
            }
            break;


        case 3:
            if(bo){
                btnState->setText(tr("Download"));
            }else{
                btnState->setText(tr("Uninstall"));
            }
            break;


        case 4://重新下载
            if(bo){
                btnState->setText(tr("Open"));
            }else{
                btnState->setText(tr("ReDownload"));
            }
            break;


        case 5://重新更新
            if(bo){
                btnState->setText(tr("Open"));
            }else{
                btnState->setText(tr("ReUpdate"));
            }
            break;
        default:
            break;

        }
    }
}


//1:下载 2:更新 3:卸载 4:重新下载 5:重新更新 6:Open
void DetailWidget::onBtnStateClicked()
{
    if(btnState->text() == "Download")
        emit sigDownload(productName,1);
    if(btnState->text() == "Update")
        emit sigUpdate(productName,2);
    if(btnState->text() == "Open")
        openApp();
    if(btnState->text() == "ReDownload")
        emit sigReDownload(productName,4);
    if(btnState->text() == "ReUpdate")
        emit sigReUpdate(productName,5);
}

void DetailWidget::openApp()
{
    QProcess proc;
    //    proc.startDetached();
}

void DetailWidget::onBtnSubmitClicked()
{
    QNetworkAccessManager *comm;
}

void DetailWidget::onBtnCancelClicked()
{
    teCommContent->clear();
}
