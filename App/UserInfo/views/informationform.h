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

public slots:
    void usersDataReady(QJsonArray data);

private slots:
    void on_lwUsers_currentRowChanged(int currentRow);

private:
    Ui::InformationForm *ui;
    QList<QTreeWidgetItem *> _items;
};

#endif // INFORMATIONFORM_H
