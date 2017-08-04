#include "classwidget.h"

#define SPACEWIDGET 5

ClassWidget::ClassWidget(QWidget *parent) :
    QWidget(parent)
{
    widget = new QWidget();
    widget->setMinimumSize(640,0);
    classtop = new ClassTop();

    gridLayout = new QGridLayout();
    gridLayout->setSpacing(24);
    gridLayout->setContentsMargins(16,0,16,0);

    mainLayout = new QVBoxLayout();
    mainLayout->addWidget(classtop->widget);
    mainLayout->addLayout(gridLayout);

    widget->setLayout(mainLayout);
    widget->installEventFilter(this);

    connect(classtop,SIGNAL(showAll(int)),this,SLOT(sendMoreShow(int)));

    spaceWidget = new QWidget[SPACEWIDGET];
    for(int i =0 ;i<5;i++)
    {
        spaceWidget[i].setFixedSize(144,74);
    }
}

ClassWidget::~ClassWidget()
{
}
//设置分类标志
void ClassWidget::setCategory(int cate)
{
    //    tt->setcategory(cate);
    classtop->setcategory(cate);
    category = cate;
}

//事件过滤器
bool ClassWidget::eventFilter(QObject *target, QEvent *event)
{
    if(target == widget)
    {
        if(event->type() == QEvent::Resize)
        {
            int numElement =0;
            int column = (widget->size().width()+48)/192;
            int row;
            //            qDebug()<<"column 1  ==  "<<column<<endl;

            if(demoElement.isEmpty())
            {
                return true;
            }

            if(demoElement.size()%column == 0)
            {
                row  = (demoElement.size()/column);
            }
            else
            {
                row  = (demoElement.size()/column)+1;
            }

            if(row > 3)
            {
                row = 3;
            }

            if(column > 6)
            {
                column = 6;
            }

            if(column < 0 || row < 0)
            {
                qDebug()<<"column or row is error!"<<endl;
            }

            if(!gridLayout->isEmpty())
            {
                //现有的控件不必清空,只是对现有的控件进行排序
                //                for(int i=0;i<demoElement.size();i++)
                //                {
                //                    while((child = gridLayout->takeAt(0))!=0)
                //                        delete child;
                //                    gridLayout->removeWidget(demoElement.at(i));

                //                }

                //空Widget每次都要清空
                for(int i = 0;i < SPACEWIDGET;i++)
                {
                    gridLayout->removeWidget(&spaceWidget[i]);
                }

            }

            //对软件控件进行重新排序
            for(int i=0;i<row;i++)
            {
                for(int j=0;j<column;j++)
                {

                    gridLayout->addWidget(demoElement.at(numElement),i,j,1,1,Qt::AlignLeft);

                    if(numElement<demoElement.size()-1)
                    {
                        numElement++;
                    }
                    else
                    {
                        break;
                    }
                }
            }

            //为不够一行的软件类添加空控件，使布局好看
            for(int i = 0;i<(column - demoElement.size());i++)
            {
//                qDebug()<<(column - demoElement.size())<<endl;
                gridLayout->addWidget(&spaceWidget[i],0,demoElement.size()+i,1,1,Qt::AlignLeft);
            }

            //隐藏多余的控件
            if(demoElement.size()>=(row*column) && column > 3)
            {

                for(int i = (row*column);i<demoElement.size();i++)
                {
                    demoElement.at(i)->hide();
                }
                for(int i = 0;i<(row*column);i++)
                {
                    demoElement.at(i)->show();
                }
            }
        }
        return true;
    }
    return QWidget::eventFilter(target,event);
}

void ClassWidget::sendMoreShow(int i)
{
    emit moreShow(i);
    //    qDebug()<<" More  Show  !!!!!!!"<<endl;
}

//设置分类项名字
void ClassWidget::setTopName(const CATEGORYMAP &cateMap)
{
    if(cateMap.isEmpty())
    {
        qDebug()<<"the cateMap is empty!"<<endl;
    }

    if(cateMap.contains(category+1))
    {
        QMap<int,QString>::const_iterator it = cateMap.find(category+1);
        classtop->setLabelData(it.value());
        //        qDebug()<<"the it.value is : "<<it.value()<<endl;
    }
}

//设置软件项名字
void ClassWidget::setElementName(const CLASSSTRUCTMAP &classStructMap)
{
    if(classStructMap.isEmpty())
    {
        qDebug()<<"the sortstr is empty!"<<endl;
    }

    QMap<int,CLASSSTRUCT>::const_iterator item = classStructMap.begin();
    for(int i = 0;item != classStructMap.end() && i<18 ; ++item)
    {
        if(item.value().category == (category+1))
        {
            tt[i].setBtnName(item.value().proName);
            tt[i].setProductId(item.key());
            tt[i].initStar(item.value().proStar);
            i++;
        }
    }
}

//初始化软件项
void ClassWidget::initElement(const ELEMENTNUMBERMAP &classElementNumMap)
{
//    qDebug()<<__FUNCTION__<<endl;
    QMap<int,int>::const_iterator it;

    it = classElementNumMap.find(category+1);
    tt = new Element[it.value()];

    for(int i=0 ; i<it.value() && i<18 ; i++)
    {
        demoElement.append(tt[i].baseWidget);
    }
}

void ClassWidget::setElementImage(const CLASSSTRUCTMAP &classStructMap)
{
    if(classStructMap.isEmpty())
    {
        qDebug()<<"the sortstr is empty!"<<endl;
    }

    QMap<int,CLASSSTRUCT>::const_iterator item = classStructMap.begin();
    for(int i = 0;item != classStructMap.end() && i<18 ; ++item)
    {
        if(item.value().category == (category+1))
        {
            tt[i].setBtnImage(item.value().proImage);
            i++;
        }
    }
}



