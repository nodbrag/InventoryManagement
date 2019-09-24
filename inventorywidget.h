#ifndef INVENTORYWIDGET_H
#define INVENTORYWIDGET_H
//author 朱志庭 26170604
#include "const.h"
#include <QWidget>
#include <QSqlQuery>
#include <QSqlQueryModel>

namespace Ui {
    class InventoryWidget;
}

class InventoryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InventoryWidget(QWidget *parent = nullptr);
    ~InventoryWidget();

private:
    Ui::InventoryWidget *ui;
    QSqlQuery *qry;
    QSqlQueryModel *model;
    QSqlQueryModel * modelName;
    QSqlQueryModel * modelUser;
    void refreshInventoryTable();
    void refreshNameComboBox();
    void refreshUserComboBox(int type,QString name);

signals:
    void inbounded();
    void outbounded();
    void refreshReport();

private slots:
    void on_pushButtonInbound_clicked();
    void on_pushButtonOutbound_clicked();
    void on_comboBoxInType_currentIndexChanged(int index);
    void on_comboBoxOutType_currentIndexChanged(int index);
};

#endif // INVENTORYWIDGET_H
