#include "titlebar.h"
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QFrame>
#include <QMouseEvent>
#include <QFocusEvent>

TitleBar::TitleBar(QWidget *parent) : QWidget(parent)
{
    resize(960,80);
    setObjectName("titleBar");

    btnBack = new QPushButton(this);
    btnNext = new QPushButton(this);
    btnRefresh = new QPushButton(this);
    btnBack->setObjectName("backBtn");
    btnNext->setObjectName("nextBtn");
    btnRefresh->setObjectName("refreshBtn");

    btnHome = new QPushButton(this);
    btnHome->setText(tr("Home"));
    btnHome->setFlat(true);
    btnClass = new QPushButton(this);
    btnClass->setText(tr("Class"));
    btnClass->setFlat(true);
    btnUpdate = new QPushButton(this);
    btnUpdate->setText(tr("Update"));
    btnUpdate->setFlat(true);
    btnManager = new QPushButton(this);
    btnManager->setText(tr("Manager"));
    btnManager->setFlat(true);

//    btnMinimum = new QPushButton(this);
//    btnMaximum = new QPushButton(this);
//    btnClose = new QPushButton(this);
//    btnMinimum->setObjectName("minBtn");
//    btnMaximum->setObjectName("maxBtn");
//    btnClose->setObjectName("closeBtn");
//    btnMinimum->resize(24,24);
//    btnMinimum->setFlat(true);
//    btnMaximum->resize(24,24);
//    btnMaximum->setFlat(true);
//    btnClose->resize(24,24);
//    btnClose->setFlat(true);


    hly6 = new QHBoxLayout;
    hly1 = new QHBoxLayout;

    hly6->setContentsMargins(0,0,0,0);

    frmSearch = new QFrame(this);
    frmSearch->setObjectName("searchFrm");
    frmSearch->setMinimumSize(QSize(160,24));

    leSearch = new CustomLineEdit(frmSearch);
    leSearch->setObjectName("searchBox");
    leSearch->setMinimumSize(QSize(160,24));
    leSearch->setFrame(false);
    leSearch->setClearButtonEnabled(true);
    leSearch->setFocusPolicy(Qt::ClickFocus);

    lbSearch = new QLabel(frmSearch);
    lbSearch->setMinimumSize(QSize(30,24));
    lbSearch->setMaximumHeight(24);
    lbSearch->setText(tr("Search"));

    lbSearchImg = new QLabel(frmSearch);
    lbSearchImg->setPixmap(QPixmap(":/image/search.png").scaledToHeight(24));
    lbSearchImg->setMinimumSize(QSize(30,24));


    connect(leSearch,SIGNAL(hideSearchLabel()),lbSearch,SLOT(hide()));
    connect(leSearch,SIGNAL(hideSearchLabel()),lbSearchImg,SLOT(hide()));
    connect(leSearch,SIGNAL(showSearchLabel()),lbSearch,SLOT(show()));
    connect(leSearch,SIGNAL(showSearchLabel()),lbSearchImg,SLOT(show()));

    hly6->addWidget(lbSearchImg);
    hly6->addWidget(lbSearch);
//    hly6->setAlignment(lbSearch,Qt::AlignCenter);
//    hly6->setAlignment(lbSearchImg,Qt::AlignCenter);
    hly1->addLayout(hly6);
    hly1->setAlignment(hly6,Qt::AlignCenter);
    hly1->setContentsMargins(0,0,0,0);
    frmSearch->setLayout(hly1);

    leSearch->raise();



//    hly1 = new QHBoxLayout;
//    hly1->addWidget(btnMinimum);
//    hly1->addWidget(btnMaximum);
//    hly1->addWidget(btnClose);
//    hly1->setSpacing(3);
//    hly1->setMargin(5);


    hly2 = new QHBoxLayout;
    hly2->addWidget(btnBack);
    hly2->addWidget(btnNext);
    hly2->addWidget(btnRefresh);

    hly3 = new QHBoxLayout;
    hly3->addWidget(btnHome);
    hly3->addWidget(btnClass);
    hly3->addWidget(btnUpdate);
    hly3->addWidget(btnManager);

//    hly4 = new QHBoxLayout;
//    hly4->addWidget(frmSearch);


    hly5 = new QHBoxLayout;
    hly5->addLayout(hly2);
    hly5->addLayout(hly3);
//    hly5->addLayout(hly4);
    hly5->addWidget(frmSearch);
    hly5->setAlignment(hly2,Qt::AlignLeft);
    hly5->setAlignment(hly3,Qt::AlignCenter);
    hly5->setAlignment(frmSearch,Qt::AlignRight);

    line0 = new QFrame(this);
    line0->setFrameShape(QFrame::HLine);


    vly = new QVBoxLayout;
//    vly->addLayout(hly1);
    vly->addLayout(hly5);
    vly->addWidget(line0);
//    vly->setAlignment(hly1,Qt::AlignRight);
    vly->setContentsMargins(10,5,8,0);

    setLayout(vly);


}


void TitleBar::mousePressEvent(QMouseEvent *event)
{
    this->setFocus();
}
