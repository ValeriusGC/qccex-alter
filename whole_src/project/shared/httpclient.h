#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
class QSslError;
class QAuthenticator;
class QNetworkReply;
QT_END_NAMESPACE

namespace storage {

QT_FORWARD_DECLARE_CLASS(Request)
QT_FORWARD_DECLARE_CLASS(Reply)

class HTTPClient : public QObject
{
    Q_OBJECT
public:
    explicit HTTPClient(QObject *parent = 0);
    ~HTTPClient();

    void startRequest(QUrl url, const Request &request);

signals:
    void fireReply(const QSharedPointer<Reply> &reply);

public slots:
    void httpFinished();
    void httpReadyRead();

private:
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
};

} // storage

#endif // HTTPCLIENT_H
