#ifndef DETAILWIDGET_H
#define DETAILWIDGET_H

#include <QObject>
#include <QWidget>
#include <qglobal.h>
class QQuickWidget;
class QLabel;
class QPushButton;
class QTextEdit;
class QHBoxLayout;
class QVBoxLayout;
class QFormLayout;

class DetailWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DetailWidget(QWidget *parent = 0);

signals:

public slots:
    void refreshPg(int);

private:
    QLabel *productImg;
    QLabel *productName;
    QLabel *comment;
    QLabel *commentStar;
    QLabel *commentMark;
    QLabel *productVersion;
    QLabel *versionNo;
    QLabel *productSize;
    QLabel *sizeValue;
    QPushButton *btnState;
    QQuickWidget *imgBoxer;
    QLabel *appInfo;
    QTextEdit *appInformation;
    QLabel *userComment;
    QLabel *userCommMark;
    QLabel *userCommStar;
    qint32 totalCommenters;
    QLabel *totalComm;
    QLabel *starLevel;
    QLabel *starLevel1;
    QLabel *starLevel2;
    QLabel *starLevel3;
    QLabel *starLevel4;
    QLabel *starLevel5;
    QLabel *myMark;
    QLabel *myStarLevel;
    QLabel *myComment;
    QTextEdit *commContent;
    QPushButton *submit;
    QPushButton *cancel;
    QQuickWidget *userCommBoxer;

    QHBoxLayout *hly1;
    QHBoxLayout *hly2;
    QHBoxLayout *hly3;
    QHBoxLayout *hly4;
    QHBoxLayout *hly5;
    QHBoxLayout *hly6;
    QHBoxLayout *hly7;
    QHBoxLayout *hly8;

    QFormLayout *fly1;
    QVBoxLayout *vly1;
    QVBoxLayout *vly2;
    QVBoxLayout *vly;

};

#endif // DETAILWIDGET_H
