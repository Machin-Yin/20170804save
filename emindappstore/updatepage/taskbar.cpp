#include "taskbar.h"

TaskBar::TaskBar(QWidget *parent,QString taskstr,QString onekeystr):QWidget(parent),taskStr(taskstr),onekeyStr(onekeystr)
{
//    this->resize(960,64);
    this->size().setHeight(64);
    hbLayout = new QHBoxLayout();
    hbLayout->setMargin(0);

    vblueLabel = new QLabel();
    vblueLabel->setStyleSheet("background:#027aff");
    vblueLabel->setFixedSize(4,16);

    taskLabel = new QLabel();
    taskLabel->setText(taskStr);
    taskLabel->setStyleSheet("font:black; font-size:16px;");

    onekeyButton = new QPushButton();
    onekeyButton->setFixedSize(80,32);
    QPalette uppal;
    uppal.setColor(QPalette::ButtonText, QColor(255,255,255));
    onekeyButton->setPalette(uppal);
    onekeyButton->setStyleSheet("border: 0px ; background: #027aff  ; border-radius: 2px; ");
    onekeyButton->setText(onekeyStr);

    hbLayout->addWidget(vblueLabel);
    hbLayout->addSpacing(2);
    hbLayout->addWidget(taskLabel);
    hbLayout->addStretch();
    hbLayout->addWidget(onekeyButton);

    setLayout(hbLayout);
}

void TaskBar::setTaskLabel(int count)
{
    QString countStr = QString::number(count, 10);
    QString taskStr = countStr + tr(" Applications Can Be Updated");
    taskLabel->setText(taskStr);
}

void TaskBar::hideOnekeyButton()
{
    onekeyButton->hide();
}

QPushButton *TaskBar::getOnekeyBtn()
{
    return onekeyButton;
}
