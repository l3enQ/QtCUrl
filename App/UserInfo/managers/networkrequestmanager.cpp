#include "networkrequestmanager.h"

#include <QtNetwork>

const QUrl host_url("http://167.99.75.222:4201/api/v1/");       // Restful server root address
const QUrl  login_url = host_url .resolved(QUrl("login"));      // users login
const QUrl logout_url = host_url .resolved(QUrl("logout"));     // users logout
const QUrl  users_url = host_url .resolved(QUrl("users"));      // users list
const QUrl   view_url = users_url.resolved(QUrl("/view/%0"));   // view   user information
const QUrl update_url = users_url.resolved(QUrl("/update/%0")); // update user information

class NetworkRequestManagerPrivate
{
public:
    QNetworkAccessManager Manager;
    QString Token;
    QThread Thread;
};


NetworkRequestManager *NetworkRequestManager::Instance()
{
    static NetworkRequestManager instance;
    return &instance;
}

void NetworkRequestManager::onLoginRequest(QString email, QString password)
{
    QJsonObject json {
        {"email", email},
        {"password", password}
    };

    QNetworkRequest req(login_url);

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QByteArray jsonData = QJsonDocument(json).toJson();

    QNetworkReply *reply = _p->Manager.post(req, jsonData);

    connect(reply, &QNetworkReply::finished, this, [=] {
        reply->deleteLater();

        const QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        const QJsonObject obj = doc.object();

        QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

        //Error handling base on response code
        if (status_code.toInt() == 200) {
            const QJsonValue  data    = obj["data"];
            const QJsonObject dataObj = data.toObject();
            qDebug() << "HTTP OK" << dataObj.keys();

            const QJsonValue token = dataObj["token"];

            if (!token.isUndefined()){
                _p->Token = token.toString();

                emit loginSuccessful(email);

                QString logMessage("login successful.");
                const QJsonValue expire = dataObj["expires_in"];
                if (!expire.isUndefined())
                    logMessage.append(QString("\nexpires in (seconds): %0").arg(expire.toDouble()));
                emit log(logMessage);

                return;

            } else emit log("No token replied in login request.");

        } else {
            const QJsonValue data  = obj["data"];
            qDebug() << "HTTP Error code: " << status_code.toInt()
                     << "Data:            " << data.toString();

            emit log(QString("HTTP Error code: %0").arg(status_code.toInt()));
        }

        emit loginFailed();
    });
}

void NetworkRequestManager::onLogoutRequest()
{
    QNetworkRequest req(logout_url);

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    const QByteArray auth = QString("Bearer %0").arg(_p->Token).toUtf8();
    req.setRawHeader(QByteArray("Authorization"), auth);

    QByteArray jsonData;
    QNetworkReply *reply = _p->Manager.post(req, jsonData);

    connect(reply, &QNetworkReply::finished, this, [=] {
        reply->deleteLater();

        QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

        //Error handling base on response code
        if (status_code.toInt() == 200)
            emit log("Logout success");
        else
            emit log("Logout didn't complete!");
    });
}

NetworkRequestManager::NetworkRequestManager(QObject *parent): QObject(parent),
    _p(new NetworkRequestManagerPrivate)
{
    _p->Thread.start();
    moveToThread(&_p->Thread);
    _p->Manager.moveToThread(&_p->Thread);
}
