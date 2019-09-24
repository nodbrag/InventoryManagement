#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "inventorywidget.h"
#include "inboundwidget.h"
#include "outboundwidget.h"
#include "userswidget.h"
#include "studentwidget.h"
#include "teacherwidget.h"
#include "reportchart.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    InventoryWidget *inventory;
    InboundWidget *inbound;
    OutboundWidget *outbound;
    UsersWidget *users;
    studentwidget *student;
    TeacherWidget *teacher;
    ReportChart * reportchart;
};

#endif // MAINWINDOW_H
