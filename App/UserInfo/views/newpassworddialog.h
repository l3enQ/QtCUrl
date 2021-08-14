#ifndef NEWPASSWORDDIALOG_H
#define NEWPASSWORDDIALOG_H

#include <QDialog>
#include <QPushButton>

namespace Ui {
class NewPasswordDialog;
}

class NewPasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewPasswordDialog(QWidget *parent = nullptr);
    ~NewPasswordDialog();

    QString getSpecifiedPassword();

private slots:
    void on_lePassword_textEdited(const QString &arg1);
    void on_lePasswordVerify_textEdited(const QString &arg1);

private:
    Ui::NewPasswordDialog *ui;
};

#endif // NEWPASSWORDDIALOG_H
