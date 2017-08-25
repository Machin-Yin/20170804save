#include "searchwidget.h"
#include <QDebug>
#define MAXNUMBER 360
#define SearchWidgetROW 72
#define SearchWidgetCOLUMN 5
#define SPACEWIDGET 5
#include <QLayout>
#include <QSpacerItem>

SearchWidget::SearchWidget(QWidget *parent,ShareData* shareData,JSONFUNC *jsonFunc) : QWidget(parent)
{

    sdata = shareData;
    jdata = jsonFunc;

    moreClassTop = new ClassTop();
    moreClassTop->setTopbtnHide();
    mainLayout = new QVBoxLayout();
    moreElement = new Element[MAXNUMBER];
    eleLayout = new QGridLayout();
    eleLayout->setSpacing(24);
    this->setMinimumSize(500,0);
    this->installEventFilter(this);
    eleLayout->setContentsMargins(16,0,16,0);
    elementNumber = 0;

    int maxEleNumber = 0;
    for(int i = 0;i<SearchWidgetROW;i++)
    {
        for(int j = 0;j<SearchWidgetCOLUMN;j++)
        {
            eleLayout->addWidget(moreElement[maxEleNumber].baseWidget,i,j,1,1,Qt::AlignLeft);
            moreElement[maxEleNumber].baseWidget->hide();
            connect(&moreElement[maxEleNumber],SIGNAL(installPackage(QString,int)),this,SLOT(sendInstallApp(QString,int)));
            connect(&moreElement[maxEleNumber],SIGNAL(updatePackage(QString,int)),this,SLOT(sendUpdateApp(QString,int)));
            if(maxEleNumber < (MAXNUMBER-1))
            {
                maxEleNumber++;
            }
            else
            {
                break;
            }
        }
    }

    categoryFlag = -1;
    mainLayout->addWidget(moreClassTop->widget);
    mainLayout->addLayout(eleLayout);

    spItem = new QSpacerItem(100,100,QSizePolicy::Expanding,QSizePolicy::Expanding);
    mainLayout->insertSpacerItem(-1,spItem);
    this->setLayout(mainLayout);

    spaceWidget = new QWidget*[SPACEWIDGET];
    for(int i =0 ;i<5;i++)
    {
        QWidget *p = new QWidget();
        spaceWidget[i] = p;
        spaceWidget[i]->setFixedSize(144,74);
    }


}

SearchWidget::~SearchWidget()
{
    delete[] moreElement;
}


void SearchWidget::setElement(QString keyText1, const CLASSSTRUCTMAP &classStruct)
{
    int showNum = 0;
    for(auto item = classStruct.begin();item != classStruct.end();++item)
    {
        if(item.value().proName.contains(keyText1))
        {
            moreElement[showNum].setBtnName(item.value().proName);
            moreElement[showNum].baseWidget->show();
            moreElement[showNum].setProductId(item.key());
            moreElement[showNum].setBtnImage(item.value().proImage);
            moreElement[showNum].setProStatus(item.value().proStatus);
            moreElement[showNum].initStar(item.value().proStar);
            moreElement[showNum].setReleaseId(item.value().releaseId);
            moreElement[showNum].setExeFile(item.value().exeFile);
            showNum++;
        }
    }

    for(int hideNum = showNum;hideNum<MAXNUMBER;hideNum++)
    {
        moreElement[hideNum].baseWidget->hide();
    }
}


//设置软件属性
void SearchWidget::setElement(int category, const CLASSSTRUCTMAP &classStruct)
{
     if(classStruct.isEmpty())
    {
        qDebug()<<"the sortstr is empty!"<<endl;
    }

    int showNum = 0;
    for(auto item = classStruct.begin();item != classStruct.end();++item)
    {
        if(item.value().category == (category+1))
        {
            moreElement[showNum].setBtnName(item.value().proName);
            moreElement[showNum].baseWidget->show();
            moreElement[showNum].setProductId(item.key());
            moreElement[showNum].setBtnImage(item.value().proImage);
            moreElement[showNum].setProStatus(item.value().proStatus);
            moreElement[showNum].initStar(item.value().proStar);
            moreElement[showNum].setReleaseId(item.value().releaseId);
            moreElement[showNum].setExeFile(item.value().exeFile);
            showNum++;
        }
    }

    for(int hideNum = showNum;hideNum<MAXNUMBER;hideNum++)
    {
        moreElement[hideNum].baseWidget->hide();
    }
}

//设置软件类名字
void SearchWidget::setTopName(QString keyText)
{
    qDebug()<<"+++++++++++++++++++"<<endl;
    moreClassTop->setLabelData(tr("Application About %1").arg(keyText));
//    categoryFlag = category;
//    if(cateGoryMap.isEmpty())
//    {
//        qDebug()<<"the cateMap is empty!"<<endl;
//    }

//    if(cateGoryMap.contains(category+1))
//    {
//        auto it = cateGoryMap.find(category+1);
//        moreClassTop->setLabelData(it.value());
//    }

}

void SearchWidget::setElementNum(const ELEMENTNUMBERMAP &elementNum)
{
    auto it = elementNum.find(categoryFlag+1);
    elementNumber = it.value();
}

bool SearchWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == this)
    {
        if(event->type() == QEvent::Resize)
        {
            if(categoryFlag == -1)
            {
                return true;
            }

            int column = (this->size().width()+48)/192;
            int row;

            if(column > 6)
            {
                column = 6;
            }

            if(elementNumber%column == 0)
            {
                row  = (elementNumber/column);
            }
            else
            {
                row  = (elementNumber/column)+1;
            }


            if(column < 0 || row < 0)
            {
                qDebug()<<"column or row is error!"<<endl;
            }

            if(!eleLayout->isEmpty())
            {
                //现有的控件不必清空,只是对现有的控件进行排序,空Widget每次都要清空
                for(int i = 0;i < 5;i++)
                {
                    eleLayout->removeWidget(spaceWidget[i]);
                }

            }

            //对软件控件进行重新排序
            int num=0;
            for(int i=0;i<row;i++)
            {
                for(int j=0;j<column;j++)
                {
                    eleLayout->addWidget(moreElement[num].baseWidget,i,j,1,1,Qt::AlignLeft);

                    if(num<(elementNumber-1))
                    {
                        num++;
                    }
                    else
                    {
                        break;
                    }
                }
            }

            //为不够一行的软件类添加空控件，使布局好看
            for(int i = 0;i<(column - elementNumber);++i)
            {
                eleLayout->addWidget(spaceWidget[i],0,elementNumber+i,1,1,Qt::AlignLeft);
            }

            //隐藏多余的控件
            if(elementNumber>column)
            {
                for(int i = elementNumber;i<MAXNUMBER;i++)
                {
                    moreElement[i].baseWidget->hide();
                }
                for(int i = 0;i<elementNumber;i++)
                {
                    moreElement[i].baseWidget->show();
                }
            }
        }
    }
    return QWidget::eventFilter(watched,event);
}

void SearchWidget::sendInstallApp(QString name, int id)
{
    emit installApp(name,id);
}

void SearchWidget::sendUpdateApp(QString name, int id)
{
    emit updateApp(name,id);
}

void SearchWidget::updatePackageStatus(QString name, bool bo,int flag)
{
    for(int i=0;i<elementNumber;i++)
    {
        if(name == moreElement[i].getBtnName())
        {
            moreElement[i].updateProStatus(bo,flag);
        }
    }
}

void SearchWidget::doSearch(QString keyText)
{
    qDebug()<<"***********"<< keyText<<endl;
    setTopName(keyText);
    setElement(keyText,sdata->classStrMap);
}

void SearchWidget::refreshPage(int)
{

}
