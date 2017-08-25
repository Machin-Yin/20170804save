#include "funcwidget.h"
#include <QDebug>

FuncWidget::FuncWidget(QWidget *parent, QString funcStr) : QWidget(parent),funcText(funcStr)
{
    funcLayout = new QHBoxLayout;

    funcLayout->setMargin(0);
    funcLayout->setSpacing(0);
    leftLayout = new QVBoxLayout;
//    leftLayout->setMargin(0);
//    leftLayout->setSpacing(0);
    rightLayout = new QVBoxLayout;
//    rightLayout->setMargin(0);
//    rightLayout->setSpacing(0);
    funcLabel = new QLabel();
    nfuncEdit = new QTextEdit();
    hideButton = new QPushButton();

    funcLabel->setText(tr(" New Features"));
    funcLabel->setStyleSheet("font: #555555; font-size:14px;");

    nfuncEdit->setText(funcStr);
    nfuncEdit->setStyleSheet("border: 0;" "background-color:transparent; font: #969696;");
    nfuncEdit->setReadOnly(true);
    nfuncEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QPixmap pixmap(":/image/hide.png");
    hideButton->setFlat(true);
    hideButton->setIcon(pixmap);
    hideButton->setIconSize(QSize(44,44));

    funcLayout->addSpacing(96);
    funcLayout->addLayout(leftLayout);
    leftLayout->addSpacing(15);
    leftLayout->addWidget(funcLabel);
    leftLayout->addSpacing(8);
    leftLayout->addWidget(nfuncEdit);
    funcLayout->addSpacing(100);
    funcLayout->addLayout(rightLayout);
    rightLayout->addStretch();
    rightLayout->addWidget(hideButton);
    rightLayout->addSpacing(10);
    funcLayout->addSpacing(10);
    setLayout(funcLayout);

}

QTextEdit *FuncWidget::getNfuncEdit()
{
    return nfuncEdit;
}

QPushButton *FuncWidget::getHideButton()
{
    return hideButton;
}

bool FuncWidget::event(QEvent *event)
{
    if(event->type() == QEvent::Paint)
    {
        int docheight = nfuncEdit->document()->size().height()-30;
        nfuncEdit->size().setHeight(docheight);
        emit sigTextHeight(docheight);
        return true;
    }
    return QWidget::event(event);
}
