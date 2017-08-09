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
    void CreateManagerWindow();
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

signals:
    void insdBtnClicked(QString);

public slots:
    void onGetInsFinished(QVariantMap insdMap);
    void onAppUpdate(QString iconUrl, QString appName, QString appVersion, QString appSize);
    void onAppUpdateOk(QString appName);
    void onAppUpdateFailure(QString appName);
    void compBtnClicked();
    void updToInsd(QString appName, QString iconUrl, QString appVer);
    void deleteRmvRow(QString pacId);
    void getPackageInstalled();
    void insManBtnClicked();
    void allStartBtnclicked();
};

#endif // MANAGERPAGE_H
