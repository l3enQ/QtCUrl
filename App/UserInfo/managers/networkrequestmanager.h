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
    void getUserList(QString userMail);
    void getUserInfo(int id, QString userMail);
    void onUpdateRequest(int id, QJsonValue item);
    void onUpdatePasswordRequest(int id, QString name, QString nickname, QString password);
    void onRefreshRequest(int id);
    void onRefreshAllRequest();

signals:
    void loginSuccessful(QString email);
    void loginFailed();
    void log(QString log);

    void usersInfoReady(QString userMail, QJsonArray items);
    void userInfoReady(int id, QString userMail, QJsonValue info);

private:
    explicit NetworkRequestManager(QObject *parent = nullptr);
    NetworkRequestManagerPrivate *_p;
};

#endif // NETWORKREQUESTMANAGER_H
