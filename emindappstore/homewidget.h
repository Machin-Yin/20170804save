#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QObject>
#include <qglobal.h>
#include <QWidget>

class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QFrame;
class QTableView;
class QQuickWidget;
class QQmlEngine;
class QScrollArea;

class HomeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HomeWidget(QWidget *parent = 0);

signals:

public slots:
    void refreshPg(int);

private:
    QHBoxLayout *hlyPicture;
    QVBoxLayout *vly;
    QLabel *lbHots;
    QFrame *line1;
    QFrame *line2;
    QTableView *tbvHots;
    QHBoxLayout *hly1;
    QQuickWidget *imgBox;
    QQmlEngine *qmlEngine;
    QScrollArea *scrollArea;
};

#endif // HOMEWIDGET_H
