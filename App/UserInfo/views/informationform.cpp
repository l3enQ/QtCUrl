#include "informationform.h"
#include "ui_informationform.h"

#include <QJsonObject>
#include <QDebug>

InformationForm::InformationForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InformationForm)
{
    ui->setupUi(this);
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

    QJsonValue item = ui->lwUsers->item(currentRow)->data(Qt::UserRole).toJsonValue();

    QTreeWidgetItem *itemGeneral = ui->treeWidget->topLevelItem(0);
    for (int child = 0; child < itemGeneral->childCount(); ++child) {
        QString key = itemGeneral->child(child)->data(0, Qt::DisplayRole).toString();
        itemGeneral->child(child)->setData(1, Qt::DisplayRole, item[key]);
    }

    QTreeWidgetItem *itemAdvanced = ui->treeWidget->topLevelItem(1);
    for (int child = 0; child < itemAdvanced->childCount(); ++child) {
        QString key = itemAdvanced->child(child)->data(0, Qt::DisplayRole).toString();
        itemAdvanced->child(child)->setData(1, Qt::DisplayRole, item[key]);
    }
}
