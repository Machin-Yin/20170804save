#include "custombutton.h"
#include <QDebug>
#include <QToolTip>
CustomButton::CustomButton()
{

}

CustomButton::~CustomButton()
{

}

void CustomButton::enterEvent(QEvent *)
{
    setStyleSheet("QPushButton{background: dodgerblue;border:1px groove;border-radius:2px;border-color:#c8c8c8;}");
}

void CustomButton::leaveEvent(QEvent *)
{
    setStyleSheet("QPushButton{border:1px groove;border-radius:2px;border-color:#c8c8c8;}");
}

