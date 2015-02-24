#ifndef BASEWND_H
#define BASEWND_H

#include <QMainWindow>
#include <QTouchEvent>
#include <QScrollBar>
#include <QModelIndex>
#include <QMenu>
#include <QTimer>
#include <netc.h>
#include <wrapper.h>

namespace Ui {
class baseWnd;
}

class baseWnd : public QMainWindow
{
    Q_OBJECT

protected:
    bool event(QEvent *event);

public:
    explicit baseWnd(QWidget *parent = 0);
    ~baseWnd();
    // -- //
    bool touchEvent(QEvent *event);

public slots:
    void refreshContent();
    void getContentText();
    void errorRequesting();

    bool swipeRecognize(QEvent *event);
    bool touchedDown(QEvent *event);
    bool touchMoved(QEvent *event);
    bool touchedUp(QEvent *event);
    void scrollDown();
    void showContextMenu();
    void scrollbarActivate();
    void createContextMenu();
    int scrollDiff(int startPoint, int endPoint);

    void showNewsList();
    void hideNewsList();
    void appendNewsList(QString httpAddr);
    void removeQueryFromList(int pos);
    void loadSelectedFeed(QModelIndex index);

signals:
    void scrolledDown();
    void scrolledUp();

private:
    Ui::baseWnd *ui;
    netC *collector;
    wrapper *xmlWrapper;
    QString content;
    QPoint *point, *pointEnd, *pointLastMove;
    QTimer* fingerTimer;
    QMenu *touchMenu;
    QAction *refreshAct, *exitAct, *scrollbarVisible, *feedsList;

    QString address;//global address of selected feen
};

#endif // BASEWND_H
