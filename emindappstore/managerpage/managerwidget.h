#ifndef MANAGERWIDGET_H
#define MANAGERWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCoreApplication>
#include <QLabel>
#include <QNetworkAccessManager>
#include <packagekitqt5/PackageKit/Transaction>
#include <packagekitqt5/PackageKit/Daemon>
#include "jsonfunc.h"
#include "sharedata.h"

enum button{
    UNINSBUTTON = 1,
    MANAGERBUTTON,
    NAMEBUTTON,
    HEADBUTTON,
};

//enum insUpFlags{
//    UPDATEFLAG,
//    INSTALLFLAG
//};

class ManagerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ManagerWidget(QWidget *parent ,QString urlstr,QString namestr, QString verstr, QString sizestr);
    void setBtnMetric(QString btnText,QPushButton *pushButton);
    void setManagerButton(QString manastr);
    QPushButton * getButton(int num);
    void setVerLabel(QString ver);
    void setSizeLabel(QString size);
    QLabel *getLabel();
    void getImage(QString headUrl);
    QString getPkgId();
    void setPkgId(QString packId);
    QString getPkgName();
    void setPkgName(QString packName);
    void setAppName(QString appname);
    QString getAppName();
    void setHeadUrl(QString iconUrl);
    QString getHeadUrl();
    void setVersion(QString version);
    QString getVersion();
    void resolveNameToId(QString packName);
    void removePackage(QString pacId);
    QString transPackSize(const double &size);
    void setFlag(QString flag);
    QString getFlag();
    void setReleaseId(int relid);
    int getReleaseId();
    void setExeFile(QString exefile);
    QString getExeFile();
    void installPackage(QString packageName);
    ShareData *shareData;
    JSONFUNC *jsonFunc;

private:
    QHBoxLayout *hbLayout;
    QVBoxLayout *vbLayout;
    QPushButton *headButton;
    QPushButton *nameButton;
    QLabel *verLabel;
    QLabel *sizeLabel;
    QPushButton *uninsButton;
    QPushButton *managerButton;
    QNetworkAccessManager *imageManager;
    QString pkgId;
    QString pkgName;
    QString appName;
    QString headUrl;
    QString appVersion;
    PackageKit::Transaction *detailTransaction;
    QString insUpFlag;
    int releaseId;
    QString exeFile;

signals:
    void pacRmvSuccess(QString);
    void pacRmvFailure(QString);
    void getDetailOk();
//    void appInstallSuccess(QString);
//    void appInstallFailure(QString);
    void resetPackageIdSuccess(QString);
    void installSuccess(QString appName);
    void installFailure(QString appName);

public slots:
    void replyFinished(QNetworkReply *reply);
    void resetPackId(PackageKit::Transaction::Info, QString packageID, QString summar);
    void removeFinished(PackageKit::Transaction::Exit status, uint runtime);
    void onGetDetails(const PackageKit::Details &value);
    void setInstallSize(QString pacId);
    void packageInstall(PackageKit::Transaction::Info, QString packageID, QString summary);
    void resolveFinished(PackageKit::Transaction::Exit status, uint runtime);
    void packageInstallFinished(PackageKit::Transaction::Exit status, uint runtime);

};

#endif // MANAGERWIDGET_H
