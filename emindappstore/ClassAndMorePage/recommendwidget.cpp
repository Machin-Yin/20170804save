#include "recommendwidget.h"
#define MAXNUMBER 36
#define RecommendWidgetROW 6
#define RecommendWidgetCOLUMN 5

RecommendWidget::RecommendWidget(QWidget *parent, ShareData *data) : QWidget(parent)
{
    classTop = new ClassTop();
    classTop->setTopbtnHide();
    mainLayout = new QVBoxLayout();
    element = new Element[MAXNUMBER];
    eleLayout = new QGridLayout();
    spaceWidget = new QWidget[5];
    spacer =new QSpacerItem(24,24,QSizePolicy::Minimum,QSizePolicy::Expanding);
    shareData = data;
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

    eleLayout->setSpacing(48);
    mainLayout->addWidget(classTop->widget);
    mainLayout->addLayout(eleLayout);
    mainLayout->addItem(spacer);
    mainLayout->setSpacing(32);
    this->setLayout(mainLayout);
    this->installEventFilter(this);
    setTopName();
}

RecommendWidget::~RecommendWidget()
{
    delete[] element;
}

//设置软件属性
void RecommendWidget::setElement(const CLASSSTRUCTMAP &classStruct,const RECOMMENDMAP &recommend)
{
    if(classStruct.isEmpty())
    {
        qDebug()<<"the sortstr is empty!"<<endl;
    }

    auto item = recommend.begin();
    QMap<int,CLASSSTRUCT>::const_iterator  item2;

    for(int i = 0;i < MAXNUMBER && item != recommend.end();++item)
    {
        item2 = classStruct.find(item.key());
        if(item2.key() == item.key())
        {
            element[i].setBtnName(item2.value().proName);
            element[i].setProductId(item2.key());
            element[i].initStar(item2.value().proStar);
            element[i].setProStatus(item2.value().proStatus);
            element[i].setBtnImage(item2.value().proImage);
            element[i].setReleaseId(item2.value().releaseId);
            element[i].setExeFile(item2.value().exeFile);
            connect(&element[i],SIGNAL(installPackage(QString,int)),this,SLOT(sendInstallApp(QString,int)));
            connect(&element[i],SIGNAL(updatePackage(QString,int)),this,SLOT(sendUpdateApp(QString,int)));
            connect(&element[i],SIGNAL(detailspageSig(int)),this,SLOT(toDetailPage(int)));
            i++;
        }
    }

    for(int hideNum = 30;hideNum < MAXNUMBER;hideNum++)
    {
        element[hideNum].baseWidget->hide();
    }
}

//设置软件类名字
void RecommendWidget::setTopName()
{
        classTop->setLabelData(tr("Recommend"));
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
        }
        else
        {
            qDebug()<<"(row * column) is error!!!"<<endl;
        }

        for(int i = 0;i<=num;i++)
        {
            element[i].baseWidget->show();
        }
        return true;
    }
    return QWidget::eventFilter(watched,event);
}

void RecommendWidget::sendInstallApp(QString name, int id)
{
    emit installApp(name,id);
}

void RecommendWidget::sendUpdateApp(QString name, int id)
{
    emit updateApp(name,id);
}

void RecommendWidget::updatePackageStatus(QString name, bool bo,int flag)
{
    for(int i=0;i<MAXNUMBER;i++)
    {
        if(name == element[i].toolTip())
        {

            if(flag == DOWNLOAD || flag == UPDATE)
            {
                element[i].updateProStatus(bo,flag);
            }
            else if(flag == UNINSTALL)
            {
                element[i].updateProStatus(bo,UNINSTALL);
            }
            else if(flag == UNINSTALLING || flag == UPDATING)
            {
                element[i].updateProStatus(bo,flag);
            }
            else if(flag == REDOWNLOAD || flag == REUPDATE)
            {
                element[i].updateProStatus(bo,flag);
            }
        }
    }
}

void RecommendWidget::toDetailPage(int id)
{
    emit toDetailSig(id);
}


