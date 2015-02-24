#ifndef NETC_H
#define NETC_H

#include <QObject>
#include <QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

class netC : public QObject
{
    Q_OBJECT
public:
    explicit netC(QString filePath, QObject *parent = 0);
    ~netC();

signals:
    void replyReceivedSignal();
    void errorWhileRequested();

public slots:
    void downloadContent();
    void replyReceived(QNetworkReply* replyObj);
    void errorOccured(QNetworkReply::NetworkError replyErr);
    QString replyString();
    QString newsErrorString();

private:
    QNetworkAccessManager *namObj;
    QNetworkRequest *namRequest;
    QString reply, errorString;
};

#endif // NETC_H
