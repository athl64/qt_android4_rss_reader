#include "basewnd.h"
#include "ui_basewnd.h"

baseWnd::baseWnd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::baseWnd)
{
    ui->setupUi(this);

    content = "";
    address = "http://k.img.com.ua/rss/ru/kyiv.xml";
    fingerTimer = new QTimer;
    this->setAttribute(Qt::WA_AcceptTouchEvents);

    point = new QPoint(this->cursor().pos().x(), this->cursor().pos().y());
    pointEnd = new QPoint(this->cursor().pos().x(), this->cursor().pos().y());
    pointLastMove = new QPoint(this->cursor().pos().x(), this->cursor().pos().y());
//    connect(this,SIGNAL(scrolledDown()),this,SLOT(refreshContent()));
//    connect(this,SIGNAL(scrolledUp()),this,SLOT(scrollDown()));
    createContextMenu();
    ui->textBrowser->verticalScrollBar()->setStyleSheet("QScrollBar::handle:vertical {background: #5C5C5C;} QScrollBar:vertical {background: transparent; width: 20px; } QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {border: solid 0px #000; width: 0px; height: 0px;}  QScrollBar::handle:vertical:pressed  {background-color: #8C2DFA;}");

    ui->textBrowser->setText("Hold finger on screen to show menu");

    //hide main news container
    connect(ui->listWidget,SIGNAL(clicked(QModelIndex)),this,SLOT(loadSelectedFeed(QModelIndex)));
    ui->textBrowser->hide();
}

baseWnd::~baseWnd()
{
    delete ui;
}

void baseWnd::refreshContent()
{
    content = "";
    if(collector) delete collector;

    //http://feeds.reuters.com/reuters/scienceNews?format=xml
    //http://k.img.com.ua/rss/ru/kyiv.xml
    collector = new netC(address);
    connect(collector,SIGNAL(replyReceivedSignal()),this,SLOT(getContentText()));
    connect(collector,SIGNAL(errorWhileRequested()),this,SLOT(errorRequesting()));
    collector->downloadContent();
    ui->textBrowser->setText("downloading news...");
}

void baseWnd::errorRequesting()
{
    ui->textBrowser->setText("Error while downloading news, check internet connection.");
}

void baseWnd::getContentText()
{
    content = collector->replyString();
    //ui->textBrowser->setText(content);
    //ui->textBrowser->setHtml(content);

    xmlWrapper = new wrapper(&content);
    QString nodesVector;
    QVector<QString> xmlNodes = xmlWrapper->getHtmlNodes();
    //nodesVector = QString::number(xmlNodes.size()) + "\n";
    for (int i = 0; i < xmlNodes.size(); i++) {
        nodesVector.append("\r\n" + xmlNodes.value(i));
    }
    ui->textBrowser->setHtml(nodesVector);
}

// = protected = //
bool baseWnd::event(QEvent *event)
{
    touchEvent(event);
    return QWidget::event(event);
}
// = protected = //

bool baseWnd::touchEvent(QEvent *event)
{
    switch(event->type())
    {
    case QEvent::MouseButtonPress :
        return touchedDown(event);
    case QEvent::MouseMove :
        return touchMoved(event);
    case QEvent::MouseButtonRelease :
        return touchedUp(event);
    default:
        return true;
    }

    return true;
}

bool baseWnd::swipeRecognize(QEvent *event)
{
    if(pointEnd->y() > point->y() + 10)
    {
        emit scrolledDown();
    }
    if(pointEnd->y() < point->y() - 10 )
    {
        emit scrolledUp();
    }
    return 0;
}

bool baseWnd::touchedDown(QEvent *event)
{
    point->setX(this->cursor().pos().x());
    point->setY(this->cursor().pos().y());
    pointLastMove->setX(this->cursor().pos().x());
    pointLastMove->setY(this->cursor().pos().y());

    fingerTimer->setSingleShot(true);
    fingerTimer->setInterval(1000);
    connect(fingerTimer,SIGNAL(timeout()),this,SLOT(showContextMenu()));
    fingerTimer->start();

    return 0;
}

bool baseWnd::touchedUp(QEvent *event)
{
    pointEnd->setX(this->cursor().pos().x());
    pointEnd->setY(this->cursor().pos().y());
    pointLastMove->setX(this->cursor().pos().x());
    pointLastMove->setY(this->cursor().pos().y());

    fingerTimer->stop();
    disconnect(fingerTimer,SIGNAL(timeout()),this,SLOT(showContextMenu()));

    swipeRecognize(event);
    return 0;
}

bool baseWnd::touchMoved(QEvent *event)
{
    ui->textBrowser->verticalScrollBar()->setValue( ui->textBrowser->verticalScrollBar()->value() + scrollDiff(pointLastMove->y(),this->cursor().pos().y()) );
    ui->textBrowser->update();

    if(fingerTimer->isActive()) {
        if(this->cursor().pos().y() > point->y() + 10)
        {
            fingerTimer->stop();
            disconnect(fingerTimer,SIGNAL(timeout()),this,SLOT(showContextMenu()));
        }
        if(this->cursor().pos().y() < point->y() - 10 )
        {
            fingerTimer->stop();
            disconnect(fingerTimer,SIGNAL(timeout()),this,SLOT(showContextMenu()));
        }
    }

    pointLastMove->setX(this->cursor().pos().x());
    pointLastMove->setY(this->cursor().pos().y());

    return 0;
}

int baseWnd::scrollDiff(int startPointY, int endPointY)
{
    if(endPointY > startPointY)
    {
        return -5;
    }
    else if(endPointY < startPointY)
    {
        return 5;
    }
    else
    {
        return 0;
    }
}

void baseWnd::scrollDown()
{
    //ui->textBrowser->scroll(0,pointEnd->y() - point->y());
}

void baseWnd::showContextMenu()
{
    touchMenu->exec(*point);
}

void baseWnd::createContextMenu()
{
    touchMenu = new QMenu(ui->textBrowser);
    refreshAct = new QAction("reload news",this);
    exitAct = new QAction("exit",this);
    feedsList = new QAction("list feeds",this);
    scrollbarVisible = new QAction("scrollbar", touchMenu);
    scrollbarVisible->setCheckable(true);
    scrollbarVisible->setChecked(true);

    connect(refreshAct,SIGNAL(triggered()),this,SLOT(refreshContent()));
    connect(exitAct,SIGNAL(triggered()),this,SLOT(close()));
    connect(scrollbarVisible,SIGNAL(triggered()),this,SLOT(scrollbarActivate()));
    connect(feedsList,SIGNAL(triggered()),this,SLOT(showNewsList()));

    touchMenu->addAction(feedsList);
    touchMenu->addAction(refreshAct);
    touchMenu->addAction(scrollbarVisible);
    touchMenu->addAction(exitAct);
    touchMenu->setStyleSheet("QMenu {font-size: 40px; color: #ffffff; background-color: #595959; border: solid 1px #9E9E9E;} QMenu::item:selected {background-color: #000000;} QMenu::indicator:checked {width: 30px; height: 30px; line-height: 30px; background-color: #8C2DFA;}");
}

void baseWnd::scrollbarActivate()
{
    if(scrollbarVisible->isChecked()) {
        ui->textBrowser->verticalScrollBar()->setVisible(true);
    }
    else
    {
        ui->textBrowser->verticalScrollBar()->setVisible(false);
    }
}

void baseWnd::showNewsList()
{
    ui->listWidget->show();

}

void baseWnd::hideNewsList()
{
    ui->listWidget->hide();
}

void baseWnd::appendNewsList(QString httpAddr)
{

}

void baseWnd::removeQueryFromList(int pos)
{

}

void baseWnd::loadSelectedFeed(QModelIndex index)
{
    hideNewsList();
    address = ui->listWidget->item(index.row())->text();
    ui->textBrowser->show();
    refreshContent();
}
