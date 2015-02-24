#include "netc.h"

netC::netC(QString filePath, QObject *parent) : QObject(parent)
{
    reply = "";
    errorString = "";
    namObj = new QNetworkAccessManager(parent);
    namRequest = new QNetworkRequest(QUrl(filePath));

    connect(namObj,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyReceived(QNetworkReply*)));
}

netC::~netC()
{
    reply.clear();
    errorString.clear();
}

void netC::downloadContent()
{
    namObj->get(*namRequest);
}

void netC::replyReceived(QNetworkReply *replyObj)
{
    if(replyObj->error())
    {
        emit errorWhileRequested();
        return;
    }
    connect(replyObj,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(errorOccured(QNetworkReply::NetworkError)));
    reply.append(replyObj->readAll());
    namObj->deleteLater();
    emit replyReceivedSignal();
}

void netC::errorOccured(QNetworkReply::NetworkError replyErr)
{
    qDebug() << "error: " << replyErr;
    errorString = "replyErr.errorString()";
    emit errorWhileRequested();
}

QString netC::replyString()
{
    return reply;
}

QString netC::newsErrorString()
{
    return errorString;
}
