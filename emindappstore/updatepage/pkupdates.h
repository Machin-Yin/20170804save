#ifndef PKUPDATES_H
#define PKUPDATES_H

#include <QObject>
#include <QPointer>
#include <QStringList>
#include <packagekitqt5/PackageKit/Transaction>
#include <packagekitqt5/PackageKit/Daemon>
#include "../jsonfunc.h"
#include "../sharedata.h"

//#define MAXUPNUM 100

class PkUpdates : public QObject
{
    Q_OBJECT
public:
    explicit PkUpdates(QObject *parent = nullptr);
    int count() const;
    int insCount() const;
    QStringList getPacName() const;
    QVariantMap packages() const;
    void installUpdate(const QString &packageId);
    void getUpdateData();
    void getMagData();

    ShareData *shareData;
    JSONFUNC *jsonFunc;
signals:
    void updatesChanged();
    void getUpdFinished(QStringList);
    void getInsFinished(QVariantMap);
    void upReleaseAry(int *,int);
    void sigUpdateData(UPDATESTRUCTMAP);
    void updateOk();

public slots:
    Q_INVOKABLE void checkUpdates(bool force = true);

private slots:
    void onFinished(PackageKit::Transaction::Exit status, uint runtime);
    void onUpdateFinished(PackageKit::Transaction::Exit status, uint runtime);
    void getUpdates();
    void getPackages();
    void onPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary);
    void onGetPackages(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary);
    void getUpRelease();
    void getInstalled();
    void sendUpdateData();
    void onPackageUpdating(PackageKit::Transaction::Info info, const QString &packageID);

private:
    void onChanged();
    void onUpdatesChanged();
    void onStatusChanged();
    QPointer<PackageKit::Transaction> m_updatesTrans;
    QPointer<PackageKit::Transaction> m_cacheTrans;
    QPointer<PackageKit::Transaction> m_packagesTrans;
    QPointer<PackageKit::Transaction> m_installTrans;
//    QPointer<PackageKit::Transaction> m_detailTrans;
    QVariantMap m_updateList;
    QVariantMap m_packagesList;
    QStringList m_upNameList;
    int sigFlag;
//    int releseAry[MAXUPNUM];

};

#endif // PKUPDATES_H
