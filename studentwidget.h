#ifndef STUDENTWIDGET_H
#define STUDENTWIDGET_H

#include "const.h"
#include <QWidget>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMenu>

namespace Ui {
class studentwidget;
}

class studentwidget : public QWidget
{
    Q_OBJECT

public:
    explicit studentwidget(QWidget *parent = nullptr);
    ~studentwidget();
private:
    Ui::studentwidget *ui;
    QSqlQuery *qry;
    QSqlQueryModel *model;
    QMenu *rightMenu;
    QAction* deleteAction ;
    QAction* updateAction;
    QString currentId;
    void refreshstudentTable();

private slots:
    void on_pushButtonEdit_clicked();
    void clicked_rightMenu(const QPoint &pos);  //右键信号槽函数
    void clicked_DeleteMenu();
    void clicked_UpdateMenu();
};

#endif // STUDENTWIDGET_H
