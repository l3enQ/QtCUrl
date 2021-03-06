#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "views/loginform.h"
#include "views/logform.h"
#include "views/informationform.h"

#include "managers/networkrequestmanager.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    LoginForm *loginForm = new LoginForm();
    ui->mainToolBar->addWidget(loginForm);

    connect(loginForm, &LoginForm::loginRequest, this, [=](QString message){
        ui->centralWidget->clear();
    });
    connect(loginForm, &LoginForm::loginRequest,
            NetworkRequestManager::Instance(), &NetworkRequestManager::onLoginRequest);
    connect(loginForm, &LoginForm::logoutRequest,
            NetworkRequestManager::Instance(), &NetworkRequestManager::onLogoutRequest);

    connect(loginForm, &LoginForm::showInfo,
            NetworkRequestManager::Instance(), &NetworkRequestManager::getUserList);

    connect(NetworkRequestManager::Instance(), &NetworkRequestManager::loginSuccessful,
            loginForm, &LoginForm::onSuccessfulLogin);
    connect(NetworkRequestManager::Instance(), &NetworkRequestManager::loginFailed,
            loginForm, &LoginForm::onLoginFailed);

    connect(NetworkRequestManager::Instance(), &NetworkRequestManager::log,
            this, [=](QString message){
        ui->centralWidget->log(message);
    });

    QDialog *infoDialog = new QDialog(this);
    infoDialog->setWindowTitle("Information");
    QVBoxLayout *layout = new QVBoxLayout(infoDialog);
    InformationForm *infoForm = new InformationForm(infoDialog);
    layout->addWidget(infoForm);

    connect(NetworkRequestManager::Instance(), &NetworkRequestManager::usersInfoReady,
            this, [=](QString userMail, QJsonArray data){
        infoForm->usersDataReady(userMail, data);
        infoDialog->show();
    });

    connect(loginForm, &LoginForm::logoutRequest, this, [=](){
        infoDialog->close();
    });

    connect(infoForm, &InformationForm::updateReq,
            NetworkRequestManager::Instance(), &NetworkRequestManager::onUpdateRequest);
    connect(infoForm, &InformationForm::updatePasswordReq,
            NetworkRequestManager::Instance(), &NetworkRequestManager::onUpdatePasswordRequest);
    connect(infoForm, &InformationForm::refreshReq,
            NetworkRequestManager::Instance(), &NetworkRequestManager::onRefreshRequest);
    connect(infoForm, &InformationForm::refreshAllReq,
            NetworkRequestManager::Instance(), &NetworkRequestManager::onRefreshAllRequest);
    connect(NetworkRequestManager::Instance(), &NetworkRequestManager::userInfoReady,
            infoForm, &InformationForm::userDataReady);
}

MainWindow::~MainWindow()
{
    delete ui;
}
