#ifndef TeacherWIDGET_H
#define TeacherWIDGET_H

#include "const.h"
#include <QWidget>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMenu>

namespace Ui {
    class TeacherWidget;
}

class TeacherWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TeacherWidget(QWidget *parent = nullptr);
    ~TeacherWidget();

private:
    Ui::TeacherWidget *ui;
    QSqlQuery *qry;
    QSqlQueryModel *model;
    QMenu *rightMenu;
    QAction* deleteAction ;
    QAction* updateAction;
    QString currentId;
    void refreshTeacherTable();

private slots:
    void on_pushButtonEdit_clicked();
    void clicked_rightMenu(const QPoint &pos);  //右键信号槽函数
    void clicked_DeleteMenu();
    void clicked_UpdateMenu();
};

#endif // TeacherWIDGET_H
