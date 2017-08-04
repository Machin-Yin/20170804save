#ifndef ELEMENTH
#define ELEMENTH
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include "custombutton.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QPaintEvent>
#include <QLabel>

class Element:public QWidget
{
    Q_OBJECT

public:
    Element();
    ~Element();

    QWidget *baseWidget;
    void setBtnImage(QString imagePath);//设置应用图标
    void setBtnName(QString name);//设置应用名字
    void setBtnStart();//设置应用评分
    void setBtnStatus(QString status);//设置应用状态
    void setButton(bool);
    int category;//应用的分类
    void setcategory(int cate);//设置应用分类
    int productId;
    void setProductId(int id);
    void initStar(double num);


private slots:
    void btnStatusSlot();
    void replyFinished(QNetworkReply *reply);

private:
    QPushButton *btnImage;
    QPushButton *btnName;
    QLabel *labelStar;

    CustomButton *btnStatus;//自定义的按钮
    QVBoxLayout *vbLayout;
    QHBoxLayout *hbLayout;
    QHBoxLayout *hbStartLayout;

    QNetworkAccessManager *m_ImageManager;
    QPixmap *m_ImagePix;

protected:
//    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *target, QEvent *event);
    int m_Flag;
    QString m_ImagePath;
};

#endif // ELEMENTH

