#include "funcwidget.h"
#include <QDebug>

FuncWidget::FuncWidget(QWidget *parent, QString funcStr) : QWidget(parent),funcText(funcStr)
{
    funcLayout = new QHBoxLayout;

    funcLayout->setMargin(0);
    leftLayout = new QVBoxLayout;
    rightLayout = new QVBoxLayout;
    funcLabel = new QLabel();
    nfuncEdit = new QTextEdit();
    hideButton = new QPushButton();

    funcLabel->setText(tr(" 新版特性"));
    nfuncEdit->setText(funcStr);
    nfuncEdit->setStyleSheet("border: 0;" "background:white");
    nfuncEdit->setReadOnly(true);
    nfuncEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QPixmap pixmap(":/image/hide.png");
    hideButton->setFlat(true);
    hideButton->setIcon(pixmap);
    hideButton->setIconSize(QSize(44,44));

    funcLayout->addSpacing(80);
    funcLayout->addLayout(leftLayout);
    leftLayout->addSpacing(20);
    leftLayout->addWidget(funcLabel);
    leftLayout->addSpacing(12);
    leftLayout->addWidget(nfuncEdit);
    leftLayout->addSpacing(24);
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
    if(event->type() == QEvent::Resize)
    {
        int docheight = nfuncEdit->document()->size().height();
        emit sigTextHeight(docheight);
        return true;
    }
    return QWidget::event(event);
}
