#include "recommendwidget.h"
#define MAXNUMBER 36
#define RecommendWidgetROW 6
#define RecommendWidgetCOLUMN 5

RecommendWidget::RecommendWidget(QWidget *parent) : QWidget(parent)
{
    classTop = new ClassTop();
    classTop->setTopbtnHide();
    mainLayout = new QVBoxLayout();
    element = new Element[MAXNUMBER];
    eleLayout = new QGridLayout();
    spaceWidget = new QWidget[5];
    spacer =new QSpacerItem(24,24,QSizePolicy::Minimum,QSizePolicy::Expanding);

    int maxEleNumber = 0;
    for(int i = 0;i<RecommendWidgetROW;i++)
    {
        for(int j = 0;j<RecommendWidgetCOLUMN;j++)
        {
            eleLayout->addWidget(element[maxEleNumber].baseWidget,i,j,1,1,Qt::AlignLeft);
//            element[maxEleNumber].baseWidget->hide();

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

    mainLayout->addWidget(classTop->widget);
    mainLayout->addLayout(eleLayout);
    mainLayout->addItem(spacer);
    this->setLayout(mainLayout);
    this->installEventFilter(this);
    setTopName();
}

//设置软件名字
void RecommendWidget::setElementName(const CLASSSTRUCTMAP &classStruct)
{
    if(classStruct.isEmpty())
    {
        qDebug()<<"the sortstr is empty!"<<endl;
    }

    QMap<int,CLASSSTRUCT>::const_iterator item = classStruct.begin();

    for(int i = 0;i < MAXNUMBER && item != classStruct.end();++item,i++)
    {
            element[i].setBtnName(item.value().proName);
            element[i].baseWidget->show();
            element[i].setProductId(item.key());
            element[i].initStar(item.value().proStar);
    }

    for(int hideNum = 30;hideNum < MAXNUMBER;hideNum++)
    {
        element[hideNum].baseWidget->hide();
    }
}

//设置软件类名字
void RecommendWidget::setTopName()
{
        classTop->setLabelData("Recommend");
}

void RecommendWidget::setElementImage(const CLASSSTRUCTMAP &classStructMap)
{
    if(classStructMap.isEmpty())
    {
        qDebug()<<"the sortstr is empty!"<<endl;
    }

    QMap<int,CLASSSTRUCT>::const_iterator item = classStructMap.begin();
    for(int i = 0;i < MAXNUMBER && item != classStructMap.end(); ++item,i++)
    {
        element[i].setBtnImage(item.value().proImage);
    }
}

bool RecommendWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::Resize)
    {
        int column = (this->size().width()+48)/192;
        int row = 6;

        if(column < 0)
        {
            qDebug()<<"column or row is error!"<<endl;
        }

        if(column > 6)
        {
            column = 6;
        }


        //对软件控件进行重新排序
        int num=0;
        for(int i=0;i<row;i++)
        {
            for(int j=0;j<column;j++)
            {
                eleLayout->addWidget(element[num].baseWidget,i,j,1,1,Qt::AlignLeft);

                if(num<((row*column)-1))
                {
                    num++;
                }
                else
                {
                    break;
                }
            }
        }

        //隐藏多余的控件
        if((row*column) <= MAXNUMBER)
        {
            for(int i = (row*column);i<MAXNUMBER;i++)
            {
                element[i].baseWidget->hide();
            }
            for(int i = 0;i<(row*column);i++)
            {
                element[i].baseWidget->show();
            }
        }
        else
        {
            qDebug()<<"(row * column) is error!!!"<<endl;
        }
        return true;
    }
    return QWidget::eventFilter(watched,event);
}

