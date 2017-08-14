#include <QDebug>
#include <QCoreApplication>
#include "pkupdates.h"

PkUpdates::PkUpdates(QObject *parent, JSONFUNC *jsonfunc) :
    QObject(parent),
    m_updatesTrans(Q_NULLPTR),
    m_cacheTrans(Q_NULLPTR),
    m_packagesTrans(Q_NULLPTR)
{    
//    shareData = new ShareData();
//    jsonFunc = new JSONFUNC(shareData);
    jsonFunc = jsonfunc;
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
    qDebug() << __FUNCTION__ << ": Got update package:" << packageID;

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
        m_packagesList.insert(packageID, summary);
    }
        break;
    default:
        break;
    }
}

void PkUpdates::onUpdateFinished(PackageKit::Transaction::Exit status, uint runtime)
{
    Q_UNUSED(runtime);
    if (status == PackageKit::Transaction::ExitSuccess)
    {
        emit updateOk();
        return;
    }
    else
    {
        emit updateFailure();
        qDebug() << "Update package didn't finish successfully";
    }

}


void PkUpdates::onFinished(PackageKit::Transaction::Exit status, uint runtime)
{
    Q_UNUSED(runtime);
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
            getUpRelease();
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
            getInstalled();

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
    qDebug()<< "jsonFunc->jsonData->classStrMap.count() :" << jsonFunc->jsonData->classStrMap.count();
    QMap<int,CLASSSTRUCT>::iterator item;
    int status;
    for(int i = 0; i < m_upNameList.count(); i++)
    {
        for(item = jsonFunc->jsonData->classStrMap.begin(); item != jsonFunc->jsonData->classStrMap.end(); item++)
        {
            if(m_upNameList.at(i) == item.value().proName)
            {
                releaseAry[num] = item.value().releaseId;
                num++;
                status = item.value().proStatus;
                if((status != REUPDATE)&&(status != REDOWNLOAD))
                {
                   item.value().proStatus = UPDATE;
                }
            }
        }
    }

    if(num != 0)
    {
        emit updateStatusChanged();
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
        for(item1 = jsonFunc->jsonData->classStrMap.begin(); item1 != jsonFunc->jsonData->classStrMap.end(); item1++)
        {
            QString packName = PackageKit::Daemon::packageName(iter.key());

            if(packName == item1.value().proName)
            {
                installedMap.insert(iter.key(),item1.value().proImage);
                int status = item1.value().proStatus;
                if(status == DOWNLOAD)
                {
                    item1.value().proStatus = OPEN;
                }
            }
        }
    }
    emit installStatusChanged();
    emit getInsFinished(installedMap);
}

void PkUpdates::sendUpdateData()
{
    emit sigUpdateData(jsonFunc->jsonData->updateStrMap);
}

void PkUpdates::installUpdate(const QString &packageId)
{
//    qDebug() << __FUNCTION__ << "packageId == " << packageId;

    PackageKit::Transaction::TransactionFlag flag = PackageKit::Transaction::TransactionFlagOnlyTrusted;
    m_installTrans = PackageKit::Daemon::updatePackage(packageId, flag);

//    connect(m_installTran.data(), &PackageKit::Transaction::statusChanged, this, &PkUpdates::onStatusChanged);
    connect(m_installTrans.data(), &PackageKit::Transaction::package, this, &PkUpdates::onPackageUpdating);
    connect(m_installTrans.data(), &PackageKit::Transaction::finished, this, &PkUpdates::onUpdateFinished);

}

void PkUpdates::installPackage(QString packageName)
{
    qDebug() << __FUNCTION__ << packageName;
    PackageKit::Transaction *resolveTransaction = PackageKit::Daemon::resolve(packageName,
//                                                   PackageKit::Transaction::FilterNone);
                                                   PackageKit::Transaction::FilterArch);
    connect(resolveTransaction,
            SIGNAL(package(PackageKit::Transaction::Info,QString,QString)),
            this,
            SLOT(packageInstall(PackageKit::Transaction::Info,QString,QString)));
    connect(resolveTransaction,
            SIGNAL(finished(PackageKit::Transaction::Exit, uint)),
            this,
            SLOT(resolveFinished(PackageKit::Transaction::Exit, uint)));

}

void PkUpdates::packageInstall(PackageKit::Transaction::Info, QString packageID, QString summary)
{
    qDebug() << "packageInstall() packageID " << packageID;
    qDebug() << "packageInstall() summary " << summary;
    PackageKit::Transaction *installTransaction = PackageKit::Daemon::installPackage(packageID);
    connect(installTransaction,
            SIGNAL(finished(PackageKit::Transaction::Exit, uint)),
            this,
            SLOT(packageInstallFinished(PackageKit::Transaction::Exit, uint)));
}

void PkUpdates::resolveFinished(PackageKit::Transaction::Exit status, uint runtime)
{
    qDebug() << "testResolveFinished() status: " << status << endl;
    qDebug() << "testResolveFinished() on of seconds: " << runtime << endl;
    if (status == PackageKit::Transaction::Exit::ExitSuccess)
    {
        qDebug() << "Package Resolve Success!";
    }
    else
    {
        emit installFailure();
        qDebug() << "Package Resolve Failure!";
    }
}

void PkUpdates::packageInstallFinished(PackageKit::Transaction::Exit status, uint runtime)
{
    qDebug() << "packageInstallFinished() status: " << status << endl;
    qDebug() << "packageInstallFinished() on of seconds: " << runtime << endl;
    if (status == PackageKit::Transaction::Exit::ExitSuccess)
    {
        emit installSuccess();
        qDebug() << "Package Install Success!";
    }
    else
    {
        emit installFailure();
        qDebug() << "Package Install Failure!";
    }
}

void PkUpdates::onPackageUpdating(PackageKit::Transaction::Info info, const QString &packageID)
{
    Q_UNUSED(info);
    const uint percent = m_installTrans->percentage();
    qDebug() << "Package updating:" << packageID << "percent == " << percent;
}

QString PkUpdates::transPackSize(const double &size)
{
    double packSize = size;
    static QStringList measures;
    if (measures.isEmpty())
        measures << QCoreApplication::translate("QInstaller", "B")
                 << QCoreApplication::translate("QInstaller", "K")
                 << QCoreApplication::translate("QInstaller", "M")
                 << QCoreApplication::translate("QInstaller", "G")
                 << QCoreApplication::translate("QInstaller", "T");
    QStringListIterator it(measures);
    QString measure(it.next());
    while (packSize >= 1024.0 && it.hasNext()) {
        measure = it.next();
        packSize /= 1024.0;
    }
    return QString::fromLatin1("%1%2").arg(packSize, 0, 'f', 1).arg(measure);
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

