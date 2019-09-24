#include "studentwidget.h"
#include "ui_studentwidget.h"
#include <QtDebug>
#include <qmessagebox.h>
#include <QSqlError>

studentwidget::studentwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::studentwidget),
    qry(new QSqlQuery),
    model(new QSqlQueryModel),
    rightMenu(new QMenu),
    deleteAction(new QAction),
    updateAction(new QAction)
{
    ui->setupUi(this);
    ui->lineEditName->setMaxLength(ITEMNAME_MAX_LEN);
    ui->lineEditISno->setMaxLength(CATEGORY_MAX_LEN);
    ui->tableViewstudent->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableViewstudent->setSelectionBehavior(QAbstractItemView::SelectRows);
    QString create_teacher_table = "CREATE TABLE IF NOT EXISTS student ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,name varchar(100),sno varchar(50),  sex varchar(10), sclass varchar(100),telephone varchar(50))";
    qDebug() << create_teacher_table<<qry->exec(create_teacher_table);

   ui->tableViewstudent->setContextMenuPolicy(Qt::CustomContextMenu);  //少这句，右键没有任何反应的

   connect( ui->tableViewstudent,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(clicked_rightMenu(QPoint)));
    rightMenu = new QMenu;
    updateAction=new QAction("修改",this);
    deleteAction = new QAction("删除",this);

    connect( deleteAction,SIGNAL(triggered(bool)),this,SLOT(clicked_DeleteMenu()));
    connect( updateAction,SIGNAL(triggered(bool)),this,SLOT(clicked_UpdateMenu()));
    rightMenu->addAction(updateAction);
    rightMenu->addAction(deleteAction);

    ui->comboBoxclass->addItem("计科171");
    ui->comboBoxclass->addItem("计科172");
    ui->comboBoxSex->addItem("男");
    ui->comboBoxSex->addItem("女");

    refreshstudentTable();

}
void studentwidget::on_pushButtonEdit_clicked()
{
    qDebug() << "[student Widget] add button clicked...";
    QString name = ui->lineEditName->text();
    QString stuclass = ui->comboBoxclass->currentText();
    QString sno=ui->lineEditISno->text();
    QString telephone=ui->lineEditTelephone->text();
    QString sex=ui->comboBoxSex->currentText();

    if (!(name.length() && sno.length()))
    {
        qDebug() << "Field no input, won't student item.";
        QMessageBox::warning(this, "提醒", "请输入姓名和学号！", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    if (!(telephone.length()))
    {
        qDebug() << "Field no input, won't tearcher telephone item.";
        QMessageBox::warning(this, "提醒", "请输入联系电话！", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    if(ui->pushButtonEdit->text()=="修改"){
        QString s = "update student set name='%1',sno='%2',sex='%3',sclass='%4',telephone='%5' where id='%6'";
        QString update_student = s.arg(name, sno,sex,stuclass,telephone,this->currentId);
        bool success = qry->exec(update_student);
        qDebug() << update_student << success;
        if (success){
            ui->lineEditName->clear();
            ui->lineEditISno->clear();
            ui->lineEditTelephone->clear();
             ui->pushButtonEdit->setText("新增");
             refreshstudentTable();
            QMessageBox::about(this,"提醒","学生修改已成功!");

        }else{
            QMessageBox::warning(this,"提醒","学生修改失败!");
            QSqlError error=qry->lastError();
                qDebug()<< error.text();
        }
    }
    else
    {
        QString s = "INSERT INTO student  (name, sno,sex,sclass,telephone) VALUES ('%1', '%2','%3','%4','%5')";
        QString add_student = s.arg(name, sno,sex,stuclass,telephone);
        bool success = qry->exec(add_student);
        qDebug() << add_student << success;
        if (success){
            ui->lineEditName->clear();
            ui->lineEditISno->clear();
            ui->lineEditTelephone->clear();
            refreshstudentTable();
            QMessageBox::about(this,"提醒","学生添加已成功!");

        }else{
            QMessageBox::warning(this,"提醒","学生添加失败!");
        }
    }
}

void studentwidget::clicked_rightMenu(const QPoint &pos)
{
    rightMenu->exec(QCursor::pos());
}

void studentwidget::clicked_DeleteMenu()
{
    int row = ui->tableViewstudent->currentIndex().row();
    QString id =model->data(model->index(row,0)).toString();
    QString s="delete from student where id = '%1'";
    QString delele_student = s.arg(id);
    bool success = qry->exec(delele_student);
    if(success){
       refreshstudentTable();
       QMessageBox::warning(this, "提醒", "删除成功!", QMessageBox::Yes, QMessageBox::Yes);

    }else{
         QMessageBox::warning(this, "提醒", "删除失败!", QMessageBox::Yes, QMessageBox::Yes);
    }
}

void studentwidget::clicked_UpdateMenu()
{
    int row = ui->tableViewstudent->currentIndex().row();
    QString id =model->data(model->index(row,0)).toString();
    QString name =model->data(model->index(row,1)).toString();
    QString sno =model->data(model->index(row,2)).toString();
    QString sex =model->data(model->index(row,3)).toString();
    QString sclass =model->data(model->index(row,4)).toString();
    QString telephone =model->data(model->index(row,5)).toString();
    this->currentId=id;
    ui->lineEditName->setText(name);
    ui->lineEditISno->setText(sno);
    ui->comboBoxSex->setCurrentText(sex);
    ui->comboBoxclass->setCurrentText(sclass);
    ui->lineEditTelephone->setText(telephone);
    ui->pushButtonEdit->setText("修改");
}

void studentwidget::refreshstudentTable()
{
    qDebug() << "Refreshing student table...";
    QString s = "SELECT id %1,name AS %2, sno AS %3, sex as %4, sclass AS %5 ,telephone as %6  FROM student";
    QString view_student = s.arg(tr("编号"),tr("名称"), tr("学号"),tr("性别"), tr("班级"),tr("电话"));
    model->setQuery(view_student);
    ui->tableViewstudent->setModel(model);
}

studentwidget::~studentwidget()
{
    delete ui;
    delete qry;
    delete model;
    delete rightMenu;
    delete deleteAction;
    delete updateAction;
}

