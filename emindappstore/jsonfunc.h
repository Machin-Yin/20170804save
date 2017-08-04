#ifndef JSONFUNCH
#define JSONFUNCH

#include <QObject>
#include <QProcess>
#include <QByteArray>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include "sharedata.h"

#define CATEGORIES  1
#define RELEASE     2
#define PRODUCTS    3
#define RECOMMEND   4
#define SCRIMAGE    5
#define COMMENTDATA 6

class JSONFUNC : public QObject
{
    Q_OBJECT

public:
    JSONFUNC(ShareData *shareData);
    void getCategoryNum();
    void getUpdateRelease(int *array, int size);
    void setAppname();
    void getRecommend();
    void getScreenImage();
    void getComment();

    ShareData *jsonData;

protected slots:
    void jsonAnalysis(QNetworkReply *reply);

signals:
    void productIsOk();
    void categoryIsOk(int num);
    void updateIsOk();
    void recommendIsOk();

private:
    int jsonFlag;
    int categoryNum;
    QByteArray testArray;
    QNetworkAccessManager *manager;
    void getCategoryNum(QJsonObject obj);
    void getProducts(QJsonObject obj);
    void getUpdateRelease(QJsonObject obj);
    void getRecommend(QJsonObject obj);
    void getScreenImage(QJsonObject obj);
    void getComment(QJsonObject obj);

    QString releaseStr;
};

#endif // JSONFUNCH
