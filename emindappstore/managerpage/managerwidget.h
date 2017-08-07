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
    void resolveNameToId(QString packName);
    void removePackage(QString pacId);
    void setInstallSize(QString pacId);
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
    PackageKit::Transaction *detailTransaction;

signals:
    void pacRmvSuccess(QString);
    void getDetailOk();

public slots:
    void replyFinished(QNetworkReply *reply);
    void resetPackId(PackageKit::Transaction::Info, QString packageID, QString summar);
    void removeFinished(PackageKit::Transaction::Exit status, uint runtime);
    void onGetDetails(const PackageKit::Details &value);

};

#endif // MANAGERWIDGET_H
