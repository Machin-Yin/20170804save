#include "morepage.h"

MorePage::MorePage(QWidget *parent) : QWidget(parent)
{
        scrollMore = new QScrollArea(this);
        scrollMore->setMaximumWidth(1200);
        showMore = new ShowMore();
        connect(showMore,SIGNAL(installApp(QString,int)),this,SLOT(sendInstallPackage(QString,int)));
        connect(showMore,SIGNAL(updateApp(QString,int)),this,SLOT(sendUpdatePackage(QString,int)));
        connect(showMore,SIGNAL(detailspageSig(int)),this,SLOT(sendDetailSig(int)));
}

MorePage::~MorePage()
{
    delete showMore;
}

bool MorePage::event(QEvent *event)
{
    if(event->type() == QEvent::Resize)
    {
        scrollMore->resize(QSize(this->width(),this->height()));
        return true;
    }
    return QWidget::event(event);
}

void MorePage::createMorewindow()
{
    pageMoreWidget = new QWidget();
    vbClasslayoutMore = new QVBoxLayout();
    scrollMore->setFrameShape(QFrame::NoFrame); //去除窗口边框
    vbClasslayoutMore->addWidget(showMore);

    pageMoreSpacer =new QSpacerItem(24,24,QSizePolicy::Minimum,QSizePolicy::Expanding);
    pageMoreWidget->setObjectName("pageMoreWidget");
    scrollMore->setWidget(pageMoreWidget);
    vbClasslayoutMore->addSpacerItem(pageMoreSpacer);
    vbClasslayoutMore->setMargin(0);
    pageMoreWidget->setLayout(vbClasslayoutMore);
    //滚动条不可见，只能通过鼠标滑动
    scrollMore->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollMore->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollMore->setWidgetResizable(true);
}

void MorePage::sendDetailSig(int id)
{
    emit detailspageSig(id);
}

void MorePage::sendInstallPackage(QString name, int id)
{
    emit installpackage(name,id);
}

void MorePage::sendUpdatePackage(QString name, int id)
{
    emit updatePackage(name,id);
}
