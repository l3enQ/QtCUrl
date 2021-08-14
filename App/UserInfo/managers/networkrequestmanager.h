#ifndef NETWORKREQUESTMANAGER_H
#define NETWORKREQUESTMANAGER_H

#include <QObject>
#include <QJsonArray>

class NetworkRequestManagerPrivate;
class NetworkRequestManager: public QObject
{
    Q_OBJECT
public:
    static NetworkRequestManager *Instance();

public slots:
    void onLoginRequest(QString email, QString password);
    void onLogoutRequest();
    void getUserList();
    void onUpdateRequest(int id, QJsonValue item);

signals:
    void loginSuccessful(QString email);
    void loginFailed();
    void log(QString log);

    void userInfoReady(QJsonArray items);

private:
    explicit NetworkRequestManager(QObject *parent = nullptr);
    NetworkRequestManagerPrivate *_p;
};

#endif // NETWORKREQUESTMANAGER_H
