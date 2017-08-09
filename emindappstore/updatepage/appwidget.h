#ifndef APPWIDGET_H
#define APPWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QNetworkAccessManager>

class AppWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AppWidget(QWidget *parent ,QString headUrl,QString nameStr,QString sizeStr,QString verStr,QString logStr, QString pkgStr);
    bool event(QEvent *event);
    void getImage(QString headUrl);
    QPushButton *getHeadButton();
    QPushButton *getNameButton();
    QPushButton *getFuncButton();
    QPushButton *getUpdateButton();
    QString getChangeLog();
    QLabel *getIntroLabel();
    QString getIntroStr();
    QString getPkgId();
    QString getHeadUrl();
    QString getAppName();
    QString getAppVer();
    QString getAppSize();

signals:
    void sigIntroResize();
    void appUpdateFinished();
    void appUpdateFailure();

public slots:
    void getImageFinished(QNetworkReply *reply);
    void onUpdateOk();
    void onUpdateFailure();
private:
    QHBoxLayout *appLayout;
    QVBoxLayout *rightLayout;
    QHBoxLayout *bottomLayout;
    QPushButton *headButton;
    QPushButton *nameButton;
    QLabel *introLabel;
    QString introstr;
    QPushButton *funcButton;
    QLabel *sizeLabel;
    QLabel *versionLabel;
    QPushButton *updateButton;
    QPixmap pix;
    QString changeLog;
    QString pkgId;
    QNetworkAccessManager *imageUpdate;
    QString iconUrl;
    QString appName;
    QString appVersion;
    QString appSize;

};

#endif // APPWIDGET_H
