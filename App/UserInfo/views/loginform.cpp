#include "loginform.h"
#include "ui_loginform.h"
#include <QRegExp>
#include <QtDebug>

LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);

    ui->leUser    ->setText("benyamin.saedi@gmail.com");
    ui->lePassword->setText("DQqcsuRUHx_2pM-");

    checkLoginValidation();
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::onSuccessfulLogin(QString email)
{
    ui->lLoginUser->setText(email);

    ui->pLogin->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->stackedWidget->setCurrentWidget(ui->pLogout);

    ui->pLogin->setEnabled(true);
}

void LoginForm::onLoginFailed()
{
    ui->pLogin->setEnabled(true);
}


void LoginForm::on_btnLogin_clicked()
{
    ui->pLogin->setDisabled(true);

    emit loginRequest(ui->leUser->text(), ui->lePassword->text());
}

void LoginForm::on_btnLogout_clicked()
{
    emit logoutRequest();

    ui->pLogin->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    ui->stackedWidget->setCurrentWidget(ui->pLogin);
}

void LoginForm::on_lePassword_textChanged(const QString &arg1)
{
    _validations.insert("password", arg1.size() >= 6);

    checkLoginValidation();
}

void LoginForm::on_leUser_textChanged(const QString &arg1)
{
    QRegExp mailRegExp("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b");
    mailRegExp.setCaseSensitivity(Qt::CaseInsensitive);
    mailRegExp.setPatternSyntax(QRegExp::RegExp);

    _validations.insert("email", mailRegExp.exactMatch(arg1));

    checkLoginValidation();
}

void LoginForm::checkLoginValidation()
{
    ui->btnLogin->setEnabled(_validations.value("email",    false) &&
                             _validations.value("password", false));
}

void LoginForm::on_leUser_returnPressed()
{
    ui->btnLogin->animateClick();
}

void LoginForm::on_lePassword_returnPressed()
{
    ui->btnLogin->animateClick();
}

void LoginForm::on_btnInfo_clicked()
{
    emit showInfo(ui->chkbxAdmin->isChecked() ? "" :
                                                ui->lLoginUser->text());
}
