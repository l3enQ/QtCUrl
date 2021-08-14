#ifndef INFORMATIONFORM_H
#define INFORMATIONFORM_H

#include <QWidget>
#include <QJsonArray>

namespace Ui {
class InformationForm;
}

class QTreeWidgetItem;
class InformationForm : public QWidget
{
    Q_OBJECT

public:
    explicit InformationForm(QWidget *parent = nullptr);
    ~InformationForm();

signals:
    void updateReq(int id, QJsonValue data);
    void refreshReq(int id);

public slots:
    void usersDataReady(QString userMail, QJsonArray data);

private slots:
    void on_lwUsers_currentRowChanged(int currentRow);

    void on_btnUpdate_clicked();

    void on_btnRefresh_clicked();

private:
    Ui::InformationForm *ui;
    QList<QTreeWidgetItem *> _items;
};

#endif // INFORMATIONFORM_H
