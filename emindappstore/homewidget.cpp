#include "homewidget.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QTableView>
#include <QQuickWidget>
#include <QtQml/QQmlApplicationEngine>
#include <QScrollArea>

HomeWidget::HomeWidget(QWidget *parent) : QWidget(parent)
{
    resize(940,640);

    hlyPicture = new QHBoxLayout;
    vly = new QVBoxLayout;
    lbHots = new QLabel(this);

    line1 = new QFrame(this);
    line1->setFrameShape(QFrame::VLine);
    line1->resize(2,16);

    line2 = new QFrame(this);
    line2->resize(900,2);
    line2->setAutoFillBackground(true);
    line2->setFrameShadow(QFrame::Raised);
    line2->setFrameShape(QFrame::HLine);

    tbvHots = new QTableView(this);

    hly1 = new QHBoxLayout;
    qmlEngine = new QQmlEngine;
    imgBox = new QQuickWidget(qmlEngine,this);
    imgBox->setMaximumSize(QSize(940,800));
    imgBox->setSource(QUrl::fromLocalFile(":/Imgbox.qml"));

    lbHots->setText(tr("RECOMMAND"));
    hly1->addWidget(line1);
    hly1->addWidget(lbHots);

    vly->addWidget(imgBox);
    vly->setAlignment(imgBox,Qt::AlignCenter);
    vly->addLayout(hly1);
    vly->setAlignment(hly1,Qt::AlignLeft);
    vly->addWidget(line2);
    vly->addWidget(tbvHots);
    setLayout(vly);

}

void HomeWidget::refreshPg(int)
{

}
