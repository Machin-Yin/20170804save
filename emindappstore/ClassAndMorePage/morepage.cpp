#include "morepage.h"

MorePage::MorePage(QWidget *parent) : QWidget(parent)
{
        scrollMore = new QScrollArea(this);
        scrollMore->setMaximumWidth(1200);
        showMore = new ShowMore();
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
    vbClasslayoutMore->addWidget(showMore->moreWidget);

    pageMoreSpacer =new QSpacerItem(24,24,QSizePolicy::Minimum,QSizePolicy::Expanding);
    scrollMore->setWidget(pageMoreWidget);
    vbClasslayoutMore->addSpacerItem(pageMoreSpacer);
    vbClasslayoutMore->setMargin(0);
    pageMoreWidget->setLayout(vbClasslayoutMore);
    //滚动条不可见，只能通过鼠标滑动
//    scrollMore->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollMore->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollMore->setWidgetResizable(true);
}
