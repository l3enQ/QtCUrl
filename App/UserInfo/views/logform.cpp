#include "logform.h"
#include "ui_logform.h"

#include <QDateTime>

LogForm::LogForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogForm)
{
    ui->setupUi(this);
}

LogForm::~LogForm()
{
    delete ui;
}

void LogForm::log(const QString &message)
{
    ui->textBrowser->append(QDateTime::currentDateTime().toString());
    ui->textBrowser->insertPlainText(" - ");
    ui->textBrowser->insertPlainText(message);
}

void LogForm::clear()
{
    ui->textBrowser->clear();
}
