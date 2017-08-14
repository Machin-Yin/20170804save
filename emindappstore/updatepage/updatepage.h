#ifndef UPDATEPAGE_H
#define UPDATEPAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QScrollArea>
#include <QPointer>
#include <QMutex>
#include <QCoreApplication>
#include <packagekitqt5/PackageKit/Daemon>
#include <packagekitqt5/PackageKit/Transaction>
#include "taskbar.h"
#include "appwidget.h"
#include "funcwidget.h"
#include "pkupdates.h"

class UpdatePage : public QWidget
{
    Q_OBJECT
public:
    explicit UpdatePage(QWidget *parent,JSONFUNC *json);
    void createUpdateWindow();
    void getUpdates();
    QString transPackSize(const double &size);

private:
    QWidget *pageUpdateWidget;
    TaskBar *updTaskBar;
    QTableWidget *updateTable;
    QVBoxLayout *upVLayout;
    QScrollArea *upScroArea;
    bool event(QEvent *event);
    PkUpdates * upd;
    JSONFUNC *jsonFunc;
//    QMutex mutex;
//    int test;

signals:
    void done();
    void theUpdateApp(QString, QString, QString, QString);
    void appUpdateOk(QString, QString, QString);
    void appUpdateFailure(QString);
    void updateStatusChanged();

public slots:
    void pageUpdateBtnClicked();
    void strLenChanged();
    void textAreaChanged(int hig);
    void onGetupFinished(UPDATESTRUCTMAP updateMap);
    void oneKeyBtnclicked();
    void onAppUpdateFinished();
    void onAppUpdateFailure();
    void getPackageUpdate();
    void onInsdBtnClicked(QString appName);
    void onStatusChanged();

};

#endif // UPDATEPAGE_H
