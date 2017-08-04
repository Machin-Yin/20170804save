#include "searchwidget.h"
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QTableView>

SearchWidget::SearchWidget(QWidget *parent) : QWidget(parent)
{
    line = new QFrame(this);
    lbInfo = new QLabel(this);
    vly = new QVBoxLayout;
    space = new QSpacerItem(32,5,QSizePolicy::Maximum,QSizePolicy::Fixed);

    lbNoResult = new QLabel(this);
    tbvResult = new QTableView(this);

    vly->addItem(space);
    vly->addWidget(lbInfo);
    vly->addWidget(line);
    vly->addWidget(lbNoResult);
    vly->addWidget(tbvResult);
    setLayout(vly);
}

void SearchWidget::doSearch(QString key)
{
    lbInfo->setText(tr("Applications Contains %1").arg(key));
}

void SearchWidget::refreshPg(int)
{

}
