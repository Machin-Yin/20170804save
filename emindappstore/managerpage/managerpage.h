#ifndef MANAGERPAGE_H
#define MANAGERPAGE_H

#include <QWidget>
#include <QEvent>
#include "updatepage/taskbar.h"
#include "managerpage/managerwidget.h"
#include "../updatepage/pkupdates.h"
#include "updatepage/updatepage.h"

class ManagerPage : public QWidget
{
    Q_OBJECT
public:
    explicit ManagerPage(QWidget *parent , JSONFUNC *json, ShareData *sharedata);
    void createManagerWindow();
    bool event(QEvent *event);
    void saveConfigFile(QString headUrl, QString appName, QString appVersion, QString appSize, QString flag, QString exefile, int releaseId, QString packageName);
    void readConfigFile();

private:
    QVBoxLayout *vboxLayout;
    QVBoxLayout *vboxLayout1;
    QVBoxLayout *vboxLayout2;
    TaskBar *manTaskBar;
    TaskBar *manCompBar;
    QTableWidget *installTable;
    QTableWidget *compTable;
    QLabel *splitLabel;
    ManagerWidget *compManager;
    QScrollArea *manScroArea;
    PkUpdates *updm;
    UpdatePage *updpg;
    ShareData *shareData;    
    JSONFUNC *jsonFunc;

signals:
    void upBtnClicked(QString);
    void insBtnClicked(QString,int);
    void sigInstallSuccess(QString,bool,int);
    void appInstallOk(QString,QString,QString,QString,QString);
    void removePackageSuccess(QString,bool,int);
    void installStatusChanged();
    void sigReUpdate(QString,bool,int);
    void sigReDownload(QString,bool,int);
    void sigUninstalling(QString,bool,int);


public slots:
    void onGetInsFinished(QMap<QString, INSTALLEDSTRUCT> insdMap);
    void onAppUpdate(QString iconUrl, QString appName, QString appVersion, QString appSize, int relid, QString exefile, QString pkgname);
    void onAppUpdateOk(QString appName);
    void onAppUpdateFailure(QString appName);
    void onAppInstallSuccess(QString appName);
    void onAppInstallFailure(QString appName);
    void compBtnClicked();
    void upInsToInsd(QString appName, QString iconUrl, QString appVer, QString exefile, QString pkgName);
    void deleteRmvRow(QString pacId);
    void onRemoveFailure(QString pacId);
    void getPackageInstalled();
    void insManBtnClicked();
    void allStartBtnclicked();
    void getInstallRlease(QString appName, int releaseId);
    void onAppInstall();
    void onStatusChanged();
    void sendSigReUpdate(QString appName);
    void sendSigReDownload(QString appName);

};

#endif // MANAGERPAGE_H
