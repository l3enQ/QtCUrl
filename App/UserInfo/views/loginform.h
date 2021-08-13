#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>
#include <QMap>

namespace Ui {
class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = nullptr);
    ~LoginForm();

signals:
    void loginRequest(QString email, QString password);
    void logoutRequest();

    void showInfo();

public slots:
    void onSuccessfulLogin(QString email);
    void onLoginFailed();

private slots:
    void on_btnLogin_clicked();
    void on_btnLogout_clicked();

    void on_lePassword_textChanged(const QString &arg1);
    void on_leUser_textChanged(const QString &arg1);
    void on_leUser_returnPressed();
    void on_lePassword_returnPressed();

    void on_btnInfo_clicked();

private:
    Ui::LoginForm *ui;

    QMap<QString, bool> _validations;

    void checkLoginValidation();
};

#endif // LOGINFORM_H
