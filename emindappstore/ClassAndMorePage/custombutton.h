#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QPushButton>
#include <QEvent>

class CustomButton:public QPushButton
{
public:
    CustomButton();
    ~CustomButton();

private:
    virtual void enterEvent(QEvent*);
    virtual void leaveEvent(QEvent*);
};
#endif // MYBUTTON_H
