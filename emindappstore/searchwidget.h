#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QObject>
#include <QWidget>
#include <qglobal.h>
class QLabel;
class QFrame;
class QVBoxLayout;
class QSpacerItem;
class QTableView;

class SearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchWidget(QWidget *parent = 0);

signals:

public slots:
    void doSearch(QString key);
    void refreshPg(int);


private:
    QFrame *line;
    QLabel *lbInfo;
    QLabel *lbNoResult;
    QVBoxLayout *vly;
    QSpacerItem *space;
    QTableView *tbvResult;


};

#endif // SEARCHWIDGET_H
