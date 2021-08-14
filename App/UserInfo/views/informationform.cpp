#include "informationform.h"
#include "ui_informationform.h"

#include <QJsonObject>
#include <QStyledItemDelegate>
#include <QDebug>

class TreeWidgetDelegate: public QStyledItemDelegate {
public:
    TreeWidgetDelegate(QObject* parent=0): QStyledItemDelegate(parent) {}
    virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        if (index.column() == 0)
            return 0;

        QString key = index.model()->index(index.row(), 0, index.parent()).data().toString();
        if (key == "id")
            return 0;

        return QStyledItemDelegate::createEditor(parent, option, index);
    }
};

InformationForm::InformationForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InformationForm)
{
    ui->setupUi(this);

    ui->treeWidget->setItemDelegate(new TreeWidgetDelegate());

    // fill items for using later
    QTreeWidgetItem *itemGeneral = ui->treeWidget->topLevelItem(0);
    for (int child = 0; child < itemGeneral->childCount(); ++child) {
        _items << itemGeneral->child(child);
    }
    QTreeWidgetItem *itemAdvanced = ui->treeWidget->topLevelItem(1);
    for (int child = 0; child < itemAdvanced->childCount(); ++child) {
        _items << itemAdvanced->child(child);
    }

    foreach (auto item, _items)
        item->setFlags(item->flags() | Qt::ItemIsEditable);
}

InformationForm::~InformationForm()
{
    delete ui;
}

void InformationForm::usersDataReady(QJsonArray data)
{
    ui->lwUsers->clear();

    foreach(const QJsonValue &item, data){
        QListWidgetItem *lwItem =
                new QListWidgetItem(QString("ID: %1, Name: %2")
                                    .arg(item["id"].toInt())
                .arg(item["name"].toString()));

        lwItem->setData(Qt::UserRole, item);

        ui->lwUsers->addItem(lwItem);
    }

    ui->lwUsers->setCurrentRow(0);
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
