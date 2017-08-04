#include "detailwidget.h"
#include <QQuickWidget>
#include <QLabel>
#include <QPushButton>
#include <QQuickWidget>
#include <QTextEdit>

DetailWidget::DetailWidget(QWidget *parent) : QWidget(parent)
{
    productImg = new QLabel(this);
    productName = new QLabel(this);
    comment = new QLabel(this);
    commentStar = new QLabel(this);
    commentMark = new QLabel(this);
    productVersion = new QLabel(this);
    versionNo = new QLabel(this);
    productSize = new QLabel(this);
    sizeValue = new QLabel(this);
    btnState = new QPushButton(this);
    imgBoxer = new QQuickWidget(this);
    appInfo = new QLabel(this);
    appInformation = new QTextEdit(this);
    userComment = new QLabel(this);
    userCommMark = new QLabel(this);
    userCommStar = new QLabel(this);
    totalComm = new QLabel(this);
    starLevel = new QLabel(this);
    starLevel1 = new QLabel(this);
    starLevel2 = new QLabel(this);
    starLevel3 = new QLabel(this);
    starLevel4 = new QLabel(this);
    starLevel5 = new QLabel(this);
    myMark = new QLabel(this);
    myStarLevel = new QLabel(this);
    myComment = new QLabel(this);
    commContent = new QTextEdit(this);
    submit = new QPushButton(this);
    cancel = new QPushButton(this);
    userCommBoxer = new QQuickWidget(this);

    imgBoxer = new QQuickWidget;
    imgBoxer->setSource(QUrl::fromLocalFile(":/Imgboxer.qml"));
}

void DetailWidget::refreshPg(int)
{

}
