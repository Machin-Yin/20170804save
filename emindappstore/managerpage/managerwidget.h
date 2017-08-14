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


enum{
    UNINSBUTTON = 1,
    MANAGERBUTTON,
    NAMEBUTTON,
    HEADBUTTON,
};

class ManagerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ManagerWidget(QWidget *parent ,QString urlstr,QString namestr, QString verstr, QString sizestr);
    void setManagerButton(QString manastr);
    QPushButton * getButton(int num);
    QLabel *getLabel();
    void getImage(QString headUrl);
    QString getPkgId();
    void setPkgId(QString packId);
    QString getAppName();
    QString getHeadUrl();
    QString getVersion();
    void resolveNameToId(QString packName);
    void removePackage(QString pacId);
    QString transPackSize(const double &size);

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
    QString appName;
    QString headUrl;
    QString appVersion;
    PackageKit::Transaction *detailTransaction;

signals:
    void pacRmvSuccess(QString);
    void getDetailOk();
    void appInstallSuccess(QString);
    void appInstallFailure(QString);
    void resetPackageIdSuccess(QString);

public slots:
    void replyFinished(QNetworkReply *reply);
    void resetPackId(PackageKit::Transaction::Info, QString packageID, QString summar);
    void removeFinished(PackageKit::Transaction::Exit status, uint runtime);
    void onGetDetails(const PackageKit::Details &value);
    void onInstallSuccess();
    void onInstallFailure();
    void setInstallSize(QString pacId);

};

#endif // MANAGERWIDGET_H
