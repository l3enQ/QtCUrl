#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "views/loginform.h"
#include "views/logform.h"

#include "managers/networkrequestmanager.h"

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

    connect(NetworkRequestManager::Instance(), &NetworkRequestManager::loginSuccessful,
            loginForm, &LoginForm::onSuccessfulLogin);
    connect(NetworkRequestManager::Instance(), &NetworkRequestManager::loginFailed,
            loginForm, &LoginForm::onLoginFailed);

    connect(NetworkRequestManager::Instance(), &NetworkRequestManager::log,
            this, [=](QString message){
        ui->centralWidget->log(message);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
