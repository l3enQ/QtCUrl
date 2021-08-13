#ifndef NETWORKREQUESTMANAGER_H
#define NETWORKREQUESTMANAGER_H

#include <QObject>

class NetworkRequestManagerPrivate;
class NetworkRequestManager: public QObject
{
    Q_OBJECT
public:
    static NetworkRequestManager *Instance();

public slots:
    void onLoginRequest(QString email, QString password);
    void onLogoutRequest();

signals:
    void loginSuccessful(QString email);
    void loginFailed();
    void log(QString log);

private:
    explicit NetworkRequestManager(QObject *parent = nullptr);
    NetworkRequestManagerPrivate *_p;
};

#endif // NETWORKREQUESTMANAGER_H
