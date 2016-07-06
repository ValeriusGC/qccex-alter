#include <QtNetwork>
#include <QJsonDocument>

#include "httpclient.h"
#include "shared_def.h"

#include "request.h"
#include "reply.h"



using namespace storage;

HTTPClient::HTTPClient(QObject *parent) : QObject(parent)
{

}

HTTPClient::~HTTPClient()
{

}

void HTTPClient::startRequest(QUrl url, const Request &request)
{
    QUrlQuery postData;
    LOG_TP( request.toHttpJson() );
    postData.addQueryItem("data", request.toHttpJson());
    QNetworkRequest nr(url);
    nr.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    LOG_TP(request.toJson());

    reply = qnam.post(nr, postData.toString(QUrl::FullyEncoded).toUtf8());
    connect(reply, SIGNAL(finished()),
            this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));



    /*
     *

QByteArray jsonString = "{\"method\":\"AuthenticatePlain\",\"loginName\":\"username@domain.com\",\"password\":\"mypass\"}";

// For your "Content-Length" header
QByteArray postDataSize = QByteArray::number(jsonString.size());

// Time for building your request
QUrl serviceURL("https://www.superService.com/api/1.7/ssapi.asmx");
QNetworkRequest request(serviceURL);

// Add the headers specifying their names and their values with the following method : void QNetworkRequest::setRawHeader(const QByteArray & headerName, const QByteArray & headerValue);
request.setRawHeader("User-Agent", "My app name v0.1");
request.setRawHeader("X-Custom-User-Agent", "My app name v0.1");
request.setRawHeader("Content-Type", "application/json");
request.setRawHeader("Content-Length", postDataSize);

// Use QNetworkReply * QNetworkAccessManager::post(const QNetworkRequest & request, const QByteArray & data); to send your request. Qt will rearrange everything correctly.
QNetworkReply * reply = m_qnam->post(request, jsonString);

*
     *
     *
    QByteArray postDataSize = QByteArray::number(jsonString.size());

    QUrl req("webpagehere");
    req.setQuery("Key=XXXXXXXXXXXXX");

    QNetworkRequest request(req);

    request.setRawHeader("User-Agent", "Test");
    request.setRawHeader("X-Custom-User-Agent", "Test");
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Length", postDataSize);

    QNetworkAccessManager test;

    QEventLoop loop;
    connect(&test, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    QNetworkReply * reply = test.post(request, jsonString);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));
    loop.exec();

    QByteArray response = reply->readAll();
    qDebug() << response;     *
     *
      */
}

void HTTPClient::httpFinished()
{
    reply->deleteLater();
}

void HTTPClient::httpReadyRead()
{
    const QString str = reply->readAll();
    LOG_TP(str);
    QSharedPointer<Reply> res(new Reply(str));
    emit fireReply(res);
    return;
}

