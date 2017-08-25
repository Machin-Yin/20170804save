#include "homewidget.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QTableView>
#include <QQuickWidget>
#include <QQmlEngine>
#include <QScrollArea>
#include "ClassAndMorePage/recommendwidget.h"
#include "sharedata.h"

HomeWidget::HomeWidget(QWidget *parent,ShareData *data,JSONFUNC *jdata) : QWidget(parent)
{
    setMaximumWidth(1200);
    setMinimumHeight(900);
    vly = new QVBoxLayout;

    shareData = data;
    recommWidget = new RecommendWidget(this,data);
    connect(recommWidget,SIGNAL(toDetailSig(int)),this,SIGNAL(toDetailPage(int)));
    connect(jdata,SIGNAL(recommendIsOk()),this,SLOT(setElement()));

    qmlEngine = new QQmlEngine;
    imgBox = new QQuickWidget(qmlEngine,this);
    imgBox->setMaximumSize(QSize(900,800));
    imgBox->setSource(QUrl::fromLocalFile(":/Imgbox.qml"));

    vly->addWidget(imgBox);
    vly->setAlignment(imgBox,Qt::AlignCenter);
    vly->addWidget(recommWidget);
    setLayout(vly);
}

void HomeWidget::refreshPage(int)
{

}

void HomeWidget::setElement()
{
    recommWidget->setElement(shareData->classStrMap,shareData->recommendMap);
}

HomeWidget::~HomeWidget()
{
}

void HomeWidget::mousePressEvent(QMouseEvent* event)
{
    setFocus();
}

