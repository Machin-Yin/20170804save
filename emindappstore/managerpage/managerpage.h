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
    explicit ManagerPage(QWidget *parent ,JSONFUNC *json);
    void createManagerWindow();
    bool event(QEvent *event);

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
    JSONFUNC *jsonFunc;

signals:
    void insdBtnClicked(QString);
    void sigInstallSuccess(QString,bool,int);
    void appInstallOk(QString,QString,QString);
    void removePackageSuccess(QString,bool,int);
    void installStatusChanged();

public slots:
    void onGetInsFinished(QVariantMap insdMap);
    void onAppUpdate(QString iconUrl, QString appName, QString appVersion, QString appSize);
    void onAppUpdateOk(QString appName);
    void onAppUpdateFailure(QString appName);
    void onAppInstallSuccess(QString appName);
    void onAppInstallFailure(QString appName);
    void compBtnClicked();
    void updToInsd(QString appName, QString iconUrl, QString appVer);
    void deleteRmvRow(QString pacId);
    void getPackageInstalled();
    void insManBtnClicked();
    void allStartBtnclicked();
    void getInstallRlease(QString appName, int releaseId);
    void onAppInstall();
    void onStatusChanged();

};

#endif // MANAGERPAGE_H
