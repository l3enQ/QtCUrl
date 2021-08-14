#include "newpassworddialog.h"
#include "ui_newpassworddialog.h"
#include <QDebug>

NewPasswordDialog::NewPasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewPasswordDialog)
{
    ui->setupUi(this);

    setModal(true);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

NewPasswordDialog::~NewPasswordDialog()
{
    delete ui;
}

QString NewPasswordDialog::getSpecifiedPassword()
{
    return ui->lePassword->text();
}

void NewPasswordDialog::on_lePassword_textEdited(const QString &arg1)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->
            setEnabled(arg1.size() >= 6 && ui->lePasswordVerify->text() == arg1);
}

void NewPasswordDialog::on_lePasswordVerify_textEdited(const QString &arg1)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->
            setEnabled(arg1.size() >= 6 && ui->lePassword->text() == arg1);
}
