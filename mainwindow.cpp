#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    inventory(new InventoryWidget(this)),
    inbound(new InboundWidget(this)),
    outbound(new OutboundWidget(this)),
    users(new UsersWidget(this)),
    student(new studentwidget(this)),
    teacher(new TeacherWidget(this)),
    reportchart(new ReportChart(this))
{
    qDebug() << "[Main Window] Constructing...";
    ui->setupUi(this);

    ui->tabWidget->addTab(inventory, tr("当前库存"));
    ui->tabWidget->addTab(reportchart,tr("库存统计"));
    ui->tabWidget->addTab(inbound, tr("入库管理"));
    ui->tabWidget->addTab(outbound, tr("出库管理"));
    ui->tabWidget->addTab(student,tr("学生管理"));
    ui->tabWidget->addTab(teacher,tr("教师管理"));
    ui->tabWidget->addTab(users, tr("管理员用户"));



    connect(inventory, &InventoryWidget::inbounded,
            inbound, &InboundWidget::refreshCategoryComboBox);
    connect(inventory, &InventoryWidget::inbounded,
            inbound, &InboundWidget::on_pushButtonQuery_clicked);

    connect(inventory, &InventoryWidget::refreshReport,
            reportchart, &ReportChart::reloadData);
    connect(inventory, &InventoryWidget::refreshReport,
            reportchart, &ReportChart::reloadData);

    connect(inventory, &InventoryWidget::outbounded,
            outbound, &OutboundWidget::refreshCategoryComboBox);
    connect(inventory, &InventoryWidget::outbounded,
            outbound, &OutboundWidget::on_pushButtonQuery_clicked);
}

MainWindow::~MainWindow()
{
    qDebug() << "[Main Window] Destructing...";
    delete ui;
    delete inventory;
    delete inbound;
    delete outbound;
    delete users;
    delete student;
    delete teacher;
    delete reportchart;
}
