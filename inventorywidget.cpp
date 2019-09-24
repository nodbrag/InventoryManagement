#include "inventorywidget.h"
#include "ui_inventorywidget.h"
#include <QtDebug>
#include <QMessageBox>

InventoryWidget::InventoryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InventoryWidget),
    qry(new QSqlQuery),
    model(new QSqlQueryModel),
    modelName(new QSqlQueryModel),
    modelUser(new QSqlQueryModel)
{
    qDebug() << "[Inventory Widget] Constructing...";
    ui->setupUi(this);
    ui->tableViewInventory->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);

    ui->comboBoxCategory->addItem("计算机类");
    ui->comboBoxCategory->addItem("营销市场类");
    ui->comboBoxCategory->addItem("青春文学类");
    ui->comboBoxCategory->addItem("社会心理学类");

    ui->comboBoxInType->addItem("管理员上架");
    ui->comboBoxInType->addItem("教师还书");
    ui->comboBoxInType->addItem("学生还书");

    ui->comboBoxOutType->addItem("管理员下架");
    ui->comboBoxOutType->addItem("教师借书");
    ui->comboBoxOutType->addItem("学生借书");

    // If no inventory table yet, create it.
    QString create_inventory_table  = "CREATE TABLE IF NOT EXISTS Inventory ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,name varchar(100), cate varchar(100), amount int)";

    qDebug() << create_inventory_table << qry->exec(create_inventory_table);

    // If no inbound table yet, create it.
     QString create_inbound_table = "CREATE TABLE IF NOT EXISTS Inbound ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,name varchar(100), cate varchar(100), amount int,type varchar(100), operatenamesno varchar(200), time varchar(100) )";

    qDebug() << create_inbound_table << qry->exec(create_inbound_table);

    // If no outbound table yet, create it.
    QString create_outbound_table = "CREATE TABLE IF NOT EXISTS Outbound ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,name varchar(100), cate char(100), amount int,type varchar(100), operatenamesno varchar(200), time char(100))";
    qDebug() << create_outbound_table << qry->exec(create_outbound_table);

    refreshUserComboBox(0,this->ui->comboBoxInType->currentText());
    refreshUserComboBox(1,this->ui->comboBoxOutType->currentText());

    refreshNameComboBox();
    refreshInventoryTable();
}

InventoryWidget::~InventoryWidget()
{
    qDebug() << "[Inventory Widget] Destructing...";
    delete ui;
    delete qry;
    delete model;
    delete modelName;
    delete modelUser;
}

void InventoryWidget::on_pushButtonInbound_clicked()
{
    qDebug() << "[Inventory Widget] Inbound button clicked...";
    QString name = ui->lineEditInName->text();
    QString cate = ui->comboBoxCategory->currentText();
    QString type=ui->comboBoxInType->currentText();
    QString operatenamesno=ui->comboBoxInUser->currentText();
    int increment = ui->spinBoxInAmount->value();

    if (!(name.length() && cate.length()))
    {
        qDebug() << "Field no input, won't inbound item.";
        return;
    }
    // Special Syntax Addition | NOT Standard SQL | SQLite >= 3.24.0
    // See: https://www.sqlite.org/lang_UPSERT.html
    QString s = "SELECT  cate, amount FROM Inventory WHERE name='%1'";
    QString get_item_info = s.arg(name);
    qDebug() << get_item_info << qry->exec(get_item_info);
    bool success=false;
    if (!qry->first()) {

        QString s = "INSERT INTO Inventory (name,cate,amount) VALUES('%1', '%2', %3)";
        QString inbound = s.arg(name, cate, QString::number(increment));
        success = qry->exec(inbound);
        qDebug() << inbound << success;

    }
    else
    {
        QString s = "UPDATE Inventory SET amount = amount + %1 where name='%2'";
        QString inbound = s.arg( QString::number(increment),name);
        success = qry->exec(inbound);
        qDebug() << inbound << success;
    }

    if (success)
    {
        ui->lineEditInName->clear();
        ui->spinBoxInAmount->clear();
        refreshNameComboBox();
        refreshInventoryTable();
        qDebug() << "Logging inbound...";
        QString s = "INSERT INTO Inbound (name,cate,amount,type,operatenamesno,time) VALUES("
            "'%1', '%2', %3,'%4','%5', datetime('now', 'localtime'))";
        QString log_inbound = s.arg(name, cate, QString::number(increment),type,operatenamesno);
        qDebug() << log_inbound << qry->exec(log_inbound);
        inbounded();
        refreshReport();
        QMessageBox::warning(this, "提醒", "入库已成功", QMessageBox::Yes, QMessageBox::Yes);
    }

}

void InventoryWidget::on_pushButtonOutbound_clicked()
{
    qDebug() << "[Inventory Widget] Outbound button clicked...";
    QString name = ui->comboBoxName->currentText(), cate;
    QString type=ui->comboBoxOutType->currentText();
    QString operatenamesno=ui->comboBoxOutUser->currentText();
    int amount, decrement = ui->spinBoxOutAmount->value();

    // Get category and currnet amount of item.
    QString s = "SELECT cate, amount FROM Inventory WHERE name='%1'";
    QString get_item_info = s.arg(name);
    qDebug() << get_item_info << qry->exec(get_item_info);
    if (!qry->first()) {
        QMessageBox::warning(this, "提醒", "没有找到该书的库存", QMessageBox::Yes, QMessageBox::Yes);
        qDebug() << "Item not found.";
        return;
    }
    else { cate = qry->value(0).toString(); amount = qry->value(1).toInt(); }
    // Ensure amount would still be non-negtaive after outbound.
    if (amount < decrement) {
         QMessageBox::warning(this, "提醒", "没有足够的库存！", QMessageBox::Yes, QMessageBox::Yes);
        qDebug() << "Amount not enough.";
        return;
    }

    s = "UPDATE Inventory SET amount = amount - %1 WHERE name = '%2'";
    QString outbound = s.arg(QString::number(decrement), name);
    bool success = qry->exec(outbound);
    qDebug() << outbound << success;
    if (success)
    {
        ui->spinBoxInAmount->clear();
        refreshInventoryTable();
        qDebug() << "Logging outbound...";
        s = "INSERT INTO  Outbound (name,cate,amount,type,operatenamesno,time) VALUES("
            "'%1', '%2', %3,'%4','%5', datetime('now', 'localtime'))";
        QString log_outbound = s.arg(name, cate, QString::number(decrement),type,operatenamesno);
        qDebug() << log_outbound << qry->exec(log_outbound);
        outbounded();
        refreshReport();
        QMessageBox::warning(this, "提醒", "出库已成功", QMessageBox::Yes, QMessageBox::Yes);
    }
}
void InventoryWidget::refreshNameComboBox()
{
    qDebug() << "Refreshing category combo box...";
    QString get_distinct_cate = "SELECT DISTINCT name FROM Inventory";
    modelName->setQuery(get_distinct_cate);
    ui->comboBoxName->setModel(modelName);
}

void InventoryWidget::refreshUserComboBox(int type,QString name)
{
     QString get_distinct_cate;
    if(name.contains("管理员"))
    {
         get_distinct_cate = "SELECT DISTINCT name FROM Users";
    }else if(name.contains("教师")){
        get_distinct_cate = "SELECT DISTINCT name||sno FROM teacher";
    }else{
        get_distinct_cate = "SELECT DISTINCT name||sno FROM student";
    }
    modelUser->setQuery(get_distinct_cate);
    if(type==0)
    ui->comboBoxInUser->setModel(modelUser);
    else
    ui->comboBoxOutUser->setModel(modelUser);
}
void InventoryWidget::refreshInventoryTable()
{
    qDebug() << "Refreshing inventory table...";
    QString s = "SELECT name AS %1, cate AS %2, amount AS %3 FROM Inventory";
    QString view_inventory = s.arg(tr("名称"), tr("分类"), tr("数量"));
    model->setQuery(view_inventory);
    ui->tableViewInventory->setModel(model);
}

void InventoryWidget::on_comboBoxInType_currentIndexChanged(int index)
{
    refreshUserComboBox(0,this->ui->comboBoxInType->currentText());
}


void InventoryWidget::on_comboBoxOutType_currentIndexChanged(int index)
{
 refreshUserComboBox(1,this->ui->comboBoxOutType->currentText());
}
