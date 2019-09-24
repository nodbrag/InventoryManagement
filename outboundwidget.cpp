#include "const.h"
#include "outboundwidget.h"
#include "ui_outboundwidget.h"
#include <QtDebug>

OutboundWidget::OutboundWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OutboundWidget),
    qry(new QSqlQuery),
    modelOutbound(new QSqlQueryModel),
    modelCategory(new QSqlQueryModel)
{
    qDebug() << "[Outbound Widget] Constructing...";
    ui->setupUi(this);
    ui->lineEditNameKey->setMaxLength(ITEMNAME_MAX_LEN);
    ui->tableViewOutbound->horizontalHeader()->
        setSectionResizeMode(QHeaderView::Stretch);

    refreshOutboundTableView();
    refreshCategoryComboBox();
}

OutboundWidget::~OutboundWidget()
{
    qDebug() << "[Outbound Widget] Destructing...";
    delete ui;
    delete qry;
    delete modelOutbound;
    delete modelCategory;
}

void OutboundWidget::on_pushButtonQuery_clicked()
{
    qDebug() << "[Outbound Widget] Query button clicked...";
    QString nameKey = ui->lineEditNameKey->text();
    QString category = ui->comboBoxCategory->currentText();
    bool searchName = ui->checkBoxName->isChecked();
    bool categoryOnly = ui->checkBoxCate->isChecked();

    QString clause = "";  // Prepare query where cluase if needed.
    QString nameCondition = QString("name LIKE '%%1%'").arg(nameKey);
    QString cateCondition = QString("cate = '%1'").arg(category);
    if (searchName)
    {
        clause += " WHERE " + nameCondition;
        if (categoryOnly) clause += " AND " + cateCondition;
    }
    else if (categoryOnly) clause += " WHERE " + cateCondition;

    refreshOutboundTableView(clause);
}

void OutboundWidget::refreshOutboundTableView(const QString& qry_clause/*=""*/)
{
    qDebug() << "Refreshing outbound table view...";
    QString s = "SELECT name AS %1, cate AS %2, "
        "amount as %3,type as %4,operatenamesno as %5, time as %6 FROM Outbound";
    QString view_outbound = s.arg(
        tr("名称"), tr("分类"), tr("出库数量"), tr("出库类型"),tr("出库人员"), tr("出库时间")) + qry_clause;
    qDebug() << view_outbound;
    modelOutbound->setQuery(view_outbound);
    ui->tableViewOutbound->setModel(modelOutbound);
}

void OutboundWidget::refreshCategoryComboBox()
{
    qDebug() << "Refreshing category combo box...";
    QString get_distinct_cate = "SELECT DISTINCT cate FROM Outbound";
    modelCategory->setQuery(get_distinct_cate);
    ui->comboBoxCategory->setModel(modelCategory);
}
