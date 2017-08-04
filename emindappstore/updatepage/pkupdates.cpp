#include <QDebug>
#include "pkupdates.h"

PkUpdates::PkUpdates(QObject *parent) :
    QObject(parent),
    m_updatesTrans(Q_NULLPTR),
    m_cacheTrans(Q_NULLPTR),
    m_packagesTrans(Q_NULLPTR)
{    
    shareData = new ShareData();
    jsonFunc = new JSONFUNC(shareData);
//    sigFlag = 1;
//    connect(PackageKit::Daemon::global(), &PackageKit::Daemon::updatesChanged, this, &PkUpdates::onUpdatesChanged,Qt::QueuedConnection);
    //    connect(PackageKit::Daemon::global(), &PackageKit::Daemon::networkStateChanged, this, &PkUpdates::networkStateChanged);
}

void PkUpdates::checkUpdates(bool force)
{
    qDebug() << "Checking updates, forced";
    m_cacheTrans = PackageKit::Daemon::refreshCache(force);
    connect(m_cacheTrans.data(), &PackageKit::Transaction::finished, this, &PkUpdates::onFinished);
}


void PkUpdates::getUpdateData()
{
    getUpdates();
}

void PkUpdates::getMagData()
{
    getPackages();
}


//void PkUpdates::onUpdatesChanged()
//{
////    qDebug() << __FUNCTION__ << ": Updates changed, getting updates!";
//    if(sigFlag != 2)
//    {
//        getUpdates();
//        getPackages();
//    }
//    sigFlag++;
//    if(sigFlag > 10)
//    {
//        sigFlag = 3;
//    }
//}


void PkUpdates::getUpdates()
{
    m_updatesTrans = PackageKit::Daemon::getUpdates();
    m_updateList.clear();
    m_upNameList.clear();

    connect(m_updatesTrans.data(), &PackageKit::Transaction::finished, this, &PkUpdates::onFinished);
    //    connect(m_updatesTrans.data(), &PackageKit::Transaction::errorCode, this, &PkUpdates::onErrorCode);
    connect(m_updatesTrans.data(), &PackageKit::Transaction::package, this, &PkUpdates::onPackage);
    //    connect(m_updatesTrans.data(), &PackageKit::Transaction::requireRestart, this, &PkUpdates::onRequireRestart);
    //    connect(m_updatesTrans.data(), &PackageKit::Transaction::repoSignatureRequired, this, &PkUpdates::onRepoSignatureRequired);
}

void PkUpdates::getPackages()
{
    qDebug() << __FUNCTION__;
    m_packagesTrans = PackageKit::Daemon::getPackages(PackageKit::Transaction::FilterInstalled);
    m_packagesList.clear();

    connect(m_packagesTrans.data(), &PackageKit::Transaction::finished, this, &PkUpdates::onFinished);
    //    connect(m_updatesTrans.data(), &PackageKit::Transaction::errorCode, this, &PkUpdates::onErrorCode);
    connect(m_packagesTrans.data(), &PackageKit::Transaction::package, this, &PkUpdates::onGetPackages);
    //    connect(m_updatesTrans.data(), &PackageKit::Transaction::requireRestart, this, &PkUpdates::onRequireRestart);
    //    connect(m_updatesTrans.data(), &PackageKit::Transaction::repoSignatureRequired, this, &PkUpdates::onRepoSignatureRequired);
}

void PkUpdates::onPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)
{
    QString packageName = PackageKit::Transaction::packageName(packageID);
//    qDebug() << __FUNCTION__ << ": Got update package:" << packageID;

    switch (info) {
    case PackageKit::Transaction::InfoBlocked:
        return;
    default:
        break;
    }
    m_updateList.insert(packageID, summary);
    m_upNameList.append(packageName);
}

void PkUpdates::onGetPackages(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)
{
    switch (info) {
    case PackageKit::Transaction::InfoInstalled:
    {
//        QString packageName = PackageKit::Transaction::packageName(packageID);
//        qDebug() << __FUNCTION__ << ": Got packages:" << packageID;
        m_packagesList.insert(packageID, summary);
    }
        break;
    default:
        break;
    }
}


void PkUpdates::onUpdateFinished(PackageKit::Transaction::Exit status, uint runtime)
{
    if (status == PackageKit::Transaction::ExitSuccess)
    {
        emit updateOk();
        return;
    }
    else
    {
        qDebug() << "Update package didn't finish successfully";
    }

}


void PkUpdates::onFinished(PackageKit::Transaction::Exit status, uint runtime)
{
//    qDebug() << __FUNCTION__ ;
    PackageKit::Transaction * trans = qobject_cast<PackageKit::Transaction *>(sender());
    if (!trans)
        return;

    trans->deleteLater();

    if (trans->role() == PackageKit::Transaction::RoleRefreshCache)
    {
        if (status == PackageKit::Transaction::ExitSuccess)
        {
            qDebug() << "Cache transaction finished successfully";
            return;
        }
        else
        {
            qDebug() << "Cache transaction didn't finish successfully";
        }
    }
    else if (trans->role() == PackageKit::Transaction::RoleGetUpdates)
    {
        if (status == PackageKit::Transaction::ExitSuccess)
        {

            connect(jsonFunc,SIGNAL(productIsOk()),this,SLOT(getUpRelease()));
            jsonFunc->setAppname();

//            emit getUpdFinished(m_upNameList);
//            qDebug() << "Check updates transaction finished successfully";
//            qDebug() << "Total number of updates: " << count() << endl;
        }
        else
        {
            qDebug() << "Check updates transaction didn't finish successfully";
        }
    }
    else if(trans->role() == PackageKit::Transaction::RoleGetPackages)
    {
        if (status == PackageKit::Transaction::ExitSuccess)
        {
            connect(jsonFunc,SIGNAL(productIsOk()),this,SLOT(getInstalled()));
            jsonFunc->setAppname();

            qDebug() << "Get Packages Installed transaction finished successfully";
            qDebug() << "Total number of Packages Installed: " << insCount() << endl;
        }
        else
        {
            qDebug() << "Get Packages Installed transaction didn't finish successfully";
        }
    }
}


void PkUpdates::getUpRelease()
{
    qDebug()<< __FUNCTION__;
    int releaseAry[m_upNameList.count()];
    int num = 0;
    qDebug()<< "m_upNameList.count() :" << m_upNameList.count();
    qDebug()<< "shareData->classStrMap.count() :" << shareData->classStrMap.count();
    QMap<int,CLASSSTRUCT>::iterator item;
    for(int i = 0; i < m_upNameList.count(); i++)
    {
        for(item = shareData->classStrMap.begin(); item != shareData->classStrMap.end(); item++)
        {
//            qDebug()<< "m_upNameList.at(" << i << ") :" << m_upNameList.at(i);
//            qDebug()<< "item.value().proName :" << item.value().proName << endl;

            if(m_upNameList.at(i) == item.value().proName)
            {
                releaseAry[num] = item.value().releaseId;
                num++;
            }
        }
    }

//    qDebug()<< "m_upNameList.count() :" << m_upNameList.count();
//    qDebug()<< "for over:  num == " << num;

    if(num != 0)
    {
        jsonFunc->getUpdateRelease(releaseAry,num);
        connect(jsonFunc,SIGNAL(updateIsOk()),this,SLOT(sendUpdateData()));
    }
}

void PkUpdates::getInstalled()
{
    qDebug() << __FUNCTION__;
    QVariantMap installedMap;

    QMap<int,CLASSSTRUCT>::iterator item1;
    QVariantMap::iterator iter;

    for(iter = m_packagesList.begin(); iter != m_packagesList.end(); iter++)
    {
        for(item1 = shareData->classStrMap.begin(); item1 != shareData->classStrMap.end(); item1++)
        {
            QString packName = PackageKit::Daemon::packageName(iter.key());

            if(packName == item1.value().proName)
            {
                //                installedMap.insert(item1.key(),CLASSSTRUCT(item1.value().category,item1.value().releaseId,item1.value().proImage,item1.value().proName,0,item1.value().proDescription));
                installedMap.insert(iter.key(),item1.value().proImage);
            }
        }
    }

    emit getInsFinished(installedMap);

}

void PkUpdates::sendUpdateData()
{
    emit sigUpdateData(shareData->updateStrMap);
}

void PkUpdates::installUpdate(const QString &packageId)
{
    qDebug() << __FUNCTION__ << "packageId == " << packageId;

    PackageKit::Transaction::TransactionFlag flag = PackageKit::Transaction::TransactionFlagOnlyTrusted;
    m_installTrans = PackageKit::Daemon::updatePackage(packageId, flag);

//    connect(m_installTran.data(), &PackageKit::Transaction::statusChanged, this, &PkUpdates::onStatusChanged);
    connect(m_installTrans.data(), &PackageKit::Transaction::package, this, &PkUpdates::onPackageUpdating);
    connect(m_installTrans.data(), &PackageKit::Transaction::finished, this, &PkUpdates::onUpdateFinished);

}

//void PkUpdates::installUpdates(const QStringList &packageIds, bool simulate, bool untrusted)
//{
//    qCDebug(PLASMA_PK_UPDATES) << "Installing updates" << packageIds << ", simulate:" << simulate << ", untrusted:" << untrusted;

//    PackageKit::Transaction::TransactionFlags flags = PackageKit::Transaction::TransactionFlagOnlyTrusted;
//    if (simulate)
//    {
//        flags |= PackageKit::Transaction::TransactionFlagSimulate;
//    }
//    else if (untrusted)
//    {
//        flags = PackageKit::Transaction::TransactionFlagNone;
//    }

//    m_installTrans = PackageKit::Daemon::updatePackages(packageIds, flags);
//    m_installTrans->setProperty("packages", packageIds);
//    setActivity(InstallingUpdates);

//    connect(m_installTrans.data(), &PackageKit::Transaction::statusChanged, this, &PkUpdates::onStatusChanged);
//    connect(m_installTrans.data(), &PackageKit::Transaction::finished, this, &PkUpdates::onFinished);
//    connect(m_installTrans.data(), &PackageKit::Transaction::errorCode, this, &PkUpdates::onErrorCode);
//    connect(m_installTrans.data(), &PackageKit::Transaction::package, this, &PkUpdates::onPackageUpdating);
//    connect(m_installTrans.data(), &PackageKit::Transaction::requireRestart, this, &PkUpdates::onRequireRestart);
//    connect(m_installTrans.data(), &PackageKit::Transaction::repoSignatureRequired, this, &PkUpdates::onRepoSignatureRequired);
//}

void PkUpdates::onPackageUpdating(PackageKit::Transaction::Info info, const QString &packageID)
{
    const uint percent = m_installTrans->percentage();
    qDebug() << "Package updating:" << packageID << "percent == " << percent;
}


int PkUpdates::count() const
{
    return m_updateList.count();
}

int PkUpdates::insCount() const
{
    return m_packagesList.count();
}

QVariantMap PkUpdates::packages() const
{
    return m_updateList;
}

QStringList PkUpdates::getPacName() const
{
    return m_upNameList;
}

