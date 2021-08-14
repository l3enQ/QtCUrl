#include "informationform.h"
#include "ui_informationform.h"

#include "newpassworddialog.h"

#include <QJsonObject>
#include <QStyledItemDelegate>
#include <QDebug>

class NotEditableDelegate: public QStyledItemDelegate {
public:
    NotEditableDelegate(QObject* parent=0): QStyledItemDelegate(parent) {}
    virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        if (index.column() == 0)
            return 0;

        return QStyledItemDelegate::createEditor(parent, option, index);
    }
};

InformationForm::InformationForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InformationForm)
{
    ui->setupUi(this);

    ui->treeWidget->setItemDelegateForColumn(0, new NotEditableDelegate());

    // fill items for using later
    QTreeWidgetItem *itemGeneral = ui->treeWidget->topLevelItem(0);
    for (int child = 0; child < itemGeneral->childCount(); ++child) {
        _items << itemGeneral->child(child);
    }
    QTreeWidgetItem *itemAdvanced = ui->treeWidget->topLevelItem(1);
    for (int child = 0; child < itemAdvanced->childCount(); ++child) {
        _items << itemAdvanced->child(child);
    }

    foreach (auto item, _items) {
        QString updateKey = item->data(0, Qt::DisplayRole).toString();

        if (updateKey == "id" || updateKey == "email" || updateKey == "verification")
            continue;

        if (updateKey == "level")
            updateKey = "user_level";

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        item->setData(0, Qt::UserRole, updateKey);
    }
}

InformationForm::~InformationForm()
{
    delete ui;
}

void InformationForm::usersDataReady(QString userMail, QJsonArray data)
{
    int current = ui->lwUsers->currentRow();
    ui->lwUsers->clear();

    foreach (const QJsonValue &item, data) {
        QListWidgetItem *lwItem =
                new QListWidgetItem(QString("ID: %1, Name: %2")
                                    .arg(item["id"].toInt())
                .arg(item["name"].toString()));

        lwItem->setData(Qt::UserRole, item);

        ui->lwUsers->addItem(lwItem);

        if (!userMail.isEmpty() && item["email"].toString() == userMail)
            current = ui->lwUsers->row(lwItem);
    }

    if (current == -1)
        ui->lwUsers->setCurrentRow(0);
    else
        ui->lwUsers->setCurrentRow(current);

    bool isAdmin = userMail.isEmpty();
    ui->fUsers->setVisible(isAdmin);
    ui->btnRefreshAll->setVisible(isAdmin);
}

void InformationForm::userDataReady(int id, QString userMail, QJsonValue info)
{
    for (int var = 0; var < ui->lwUsers->count(); ++var) {
        QListWidgetItem *item = ui->lwUsers->item(var);
        if (item->data(Qt::UserRole).toJsonValue()["id"] == id) {
            item->setData(Qt::UserRole, info);
            item->setData(Qt::DisplayRole, QString("ID: %1, Name: %2")
                                                .arg(id).arg(info["name"].toString()));

            if (var == ui->lwUsers->currentRow()) {
                foreach (auto item, _items) {
                    QString key = item->data(0, Qt::DisplayRole).toString();
                    item->setData(1, Qt::DisplayRole, info[key].toVariant());
                }
            }

            break;
        }
    }
}

void InformationForm::on_lwUsers_currentRowChanged(int currentRow)
{
    if (currentRow < 0)
        return;

    QJsonValue itemData = ui->lwUsers->item(currentRow)->data(Qt::UserRole).toJsonValue();

    foreach (auto item, _items) {
        QString key = item->data(0, Qt::DisplayRole).toString();
        item->setData(1, Qt::DisplayRole, itemData[key].toVariant());
    }
}

void InformationForm::on_btnUpdate_clicked()
{
    int id = ui->lwUsers->currentItem()->data(Qt::UserRole).toJsonValue()["id"].toInt();

    QJsonObject itemDataObj;
    foreach (auto item, _items) {
        QString updateKey = item->data(0, Qt::UserRole).toString();
        if (updateKey.isEmpty())
            continue;

        itemDataObj.insert(updateKey, QJsonValue::fromVariant(item->data(1, Qt::DisplayRole)));
    }

    emit updateReq(id, QJsonValue(itemDataObj));
}

void InformationForm::on_btnRefresh_clicked()
{
    int id = ui->lwUsers->currentItem()->data(Qt::UserRole).toJsonValue()["id"].toInt();

    emit refreshReq(id);
}

void InformationForm::on_btnSetPassword_clicked()
{
    NewPasswordDialog *dialog = new NewPasswordDialog(this);

    if (dialog->exec() == 1) {
        QJsonValue value = ui->lwUsers->currentItem()->data(Qt::UserRole).toJsonValue();
        int id           = value["id"].toInt();
        QString name     = value["name"].toString();
        QString nickname = value["nickname"].toString();

        QString password = dialog->getSpecifiedPassword();
        emit updatePasswordReq(id, name, nickname, password);
    }

    dialog->deleteLater();
}

void InformationForm::on_btnRefreshAll_clicked()
{
    emit refreshAllReq();
}
