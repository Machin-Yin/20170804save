#include "classtop.h"
#include <QDebug>

ClassTop::ClassTop()
{
    category = 0;
    widget = new QWidget();
    btnTop = new QPushButton();
    btnTop->setFlat(true);
    labelTop = new QLabel();
    spacerTop = new QSpacerItem(0,48,QSizePolicy::Expanding,QSizePolicy::Minimum);
    labelTop->setStyleSheet("border-left-width: 3px;border-style: solid;border-left-color: rgb(65,105,225);");
    labelTop->setMaximumHeight(16);
    labelTop->setContentsMargins(8,0,0,0);

    setBtnData();
//    setLabelData();


    lineTop = new QLabel();
    lineTop->setStyleSheet("border-top-width: 1px;border-style: solid;border-top-color: rgb(220, 220, 220);");
    lineTop->setMaximumHeight(32);

    //美化布局
    hbTopLayout = new QHBoxLayout();
    hbTopLayout->addWidget(labelTop);
    hbTopLayout->addSpacerItem(spacerTop);
    hbTopLayout->addWidget(btnTop);

    vbTopLayout = new QVBoxLayout();
    vbTopLayout->setSpacing(0);
    vbTopLayout->setMargin(0);
    vbTopLayout->addLayout(hbTopLayout);
    vbTopLayout->addWidget(lineTop);
    vbTopLayout->setContentsMargins(16,0,4,0);

    widget->setLayout(vbTopLayout);
    widget->setMaximumHeight(48);
    btnTop->setFocusPolicy(Qt::NoFocus);
    btnTop->setMaximumWidth(48);
    btnTop->setObjectName("btnTop");
//    btnTop->setStyleSheet("text-align: right;");//设置按钮文字显示位置
    btnTop->setCursor(Qt::PointingHandCursor);
    connect(btnTop,SIGNAL(clicked(bool)),this,SLOT(sendSlotBtn()));
}

ClassTop::~ClassTop()
{

}

void ClassTop::setBtnData()
{
    btnTop->setText("更多  >");
}

void ClassTop::setLabelData(QString data)
{
    labelTop->setText(data);
}

void ClassTop::setcategory(int cate)
{
    category = cate;
}

void ClassTop::setTopbtnHide()
{
    btnTop->hide();
}

void ClassTop::sendSlotBtn()
{
    emit showAll(category);
//    qDebug()<<"top sort  show all!"<<endl;
}
