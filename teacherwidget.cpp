#include "teacherwidget.h"
#include "ui_teacherwidget.h"
#include <QtDebug>
#include <QMessageBox>

TeacherWidget::TeacherWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeacherWidget),
    qry(new QSqlQuery),
    model(new QSqlQueryModel),
    rightMenu(new QMenu),
    deleteAction(new QAction),
    updateAction(new QAction)
{
    qDebug() << "[teacher Widget] Constructing...";
    ui->setupUi(this);
    ui->tableViewteacher->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);
    ui->comboBoxSex->addItem("男");
    ui->comboBoxSex->addItem("女");
    ui->comboBoxCourse->addItem("程序设计与实践");
    ui->comboBoxCourse->addItem("计算机组成原理");
    ui->comboBoxCourse->addItem("计算机网络技术与应用");
    ui->comboBoxCourse->addItem("软件工程与实践");
    ui->tableViewteacher->setSelectionBehavior(QAbstractItemView::SelectRows);
    // If no teacher table yet, create it.
    QString create_teacher_table = "CREATE TABLE IF NOT EXISTS teacher ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,name varchar(100),sno varchar(50),  sex varchar(10), course varchar(100),telephone varchar(50))";
    qDebug() << create_teacher_table<<qry->exec(create_teacher_table);
    ui->tableViewteacher->setContextMenuPolicy(Qt::CustomContextMenu);  //少这句，右键没有任何反应的

    connect( ui->tableViewteacher,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(clicked_rightMenu(QPoint)));
     rightMenu = new QMenu;
     updateAction=new QAction("修改",this);
     deleteAction = new QAction("删除",this);

     connect( deleteAction,SIGNAL(triggered(bool)),this,SLOT(clicked_DeleteMenu()));
     connect( updateAction,SIGNAL(triggered(bool)),this,SLOT(clicked_UpdateMenu()));
     rightMenu->addAction(updateAction);
     rightMenu->addAction(deleteAction);
    refreshTeacherTable();
}

TeacherWidget::~TeacherWidget()
{
    qDebug() << "[teacher Widget] Destructing...";
    delete ui;
    delete qry;
    delete model;
    delete rightMenu;
    delete deleteAction;
    delete updateAction;
}

void TeacherWidget::on_pushButtonEdit_clicked()
{
    qDebug() << "[teacher Widget] teacher button clicked...";
    QString name = ui->lineEditName->text();
    QString sno = ui->lineEditISno->text();
    QString sex = ui->comboBoxSex->currentText();
    QString coures=ui->comboBoxCourse->currentText();
    QString telephone=ui->lineEditTelephone->text();

    if (!(name.length() && sno.length()))
    {
        qDebug() << "Field no input, won't tearcher item.";
        QMessageBox::warning(this, "提醒", "请输入教师姓名和学号！", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    if (!(telephone.length()))
    {
        qDebug() << "Field no input, won't tearcher telephone item.";
        QMessageBox::warning(this, "提醒", "请输入教师联系电话！", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    if(ui->pushButtonEdit->text()=="修改"){
        QString s = "update teacher set name='%1',sno='%2',sex='%3',course='%4',telephone='%5' where id='%6'";
        QString update_teacher = s.arg(name, sno,sex,coures,telephone,this->currentId);
        bool success = qry->exec(update_teacher);
        qDebug() << update_teacher << success;
        if (success){
            ui->lineEditName->clear();
            ui->lineEditISno->clear();
            ui->lineEditTelephone->clear();
             ui->pushButtonEdit->setText("新增");
             refreshTeacherTable();
            QMessageBox::about(this,"提醒","修改已成功!");

        }else{
            QMessageBox::warning(this,"提醒","修改失败!");
        }
    }
    else
    {
    QString s = "INSERT INTO teacher (name, sno,sex,course,telephone) VALUES('%1', '%2', '%3','%4','%5') ";
    QString inbound = s.arg(name, sno,sex,coures,telephone);
    bool success = qry->exec(inbound);
    qDebug() << inbound << success;
    if (success)
    {
        ui->lineEditName->clear();
        ui->lineEditISno->clear();
        ui->lineEditTelephone->clear();
          refreshTeacherTable();
        QMessageBox::about(this,"提醒","添加已成功!");

    }
    else
    {
        QMessageBox::warning(this,"提醒","添加失败!");
    }
    }
}

void TeacherWidget::clicked_rightMenu(const QPoint &pos)
{
    rightMenu->exec(QCursor::pos());
}

void TeacherWidget::clicked_DeleteMenu()
{
    int row = ui->tableViewteacher->currentIndex().row();
    QString id =model->data(model->index(row,0)).toString();
    QString s="delete from teacher where id = '%1'";
    QString delele_teacher = s.arg(id);
    bool success = qry->exec(delele_teacher);
    if(success){
       refreshTeacherTable();
       QMessageBox::warning(this, "提醒", "删除成功!", QMessageBox::Yes, QMessageBox::Yes);

    }else{
         QMessageBox::warning(this, "提醒", "删除失败!", QMessageBox::Yes, QMessageBox::Yes);
    }
}

void TeacherWidget::clicked_UpdateMenu()
{
    int row = ui->tableViewteacher->currentIndex().row();
    QString id =model->data(model->index(row,0)).toString();
    QString name =model->data(model->index(row,1)).toString();
    QString sno =model->data(model->index(row,2)).toString();
    QString sex =model->data(model->index(row,3)).toString();
    QString course =model->data(model->index(row,4)).toString();
    QString telephone =model->data(model->index(row,5)).toString();
    this->currentId=id;
    ui->lineEditName->setText(name);
    ui->lineEditISno->setText(sno);
    ui->comboBoxSex->setCurrentText(sex);
    ui->comboBoxCourse->setCurrentText(course);
    ui->lineEditTelephone->setText(telephone);
    ui->pushButtonEdit->setText("修改");
}

void TeacherWidget::refreshTeacherTable()
{
    qDebug() << "Refreshing teacher table...";
    QString s = "SELECT id as %1, name AS %2, sex AS %3,sno AS %4, course as %5,telephone as %6 FROM teacher";
    QString view_teacher = s.arg(tr("编号"),tr("教师姓名"), tr("性别"), tr("教学号"),tr("教授课程"),tr("联系电话"));
    model->setQuery(view_teacher);
    ui->tableViewteacher->setModel(model);
}
