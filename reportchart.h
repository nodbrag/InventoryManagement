#ifndef REPORTCHART_H
#define REPORTCHART_H
#include <QtCharts/QtCharts>
#include <QWidget>
#include <QSqlQuery>
#include <QSqlQueryModel>

namespace Ui {
class ReportChart;
}

class ReportChart : public QWidget
{
    Q_OBJECT

public:
    explicit ReportChart(QWidget *parent = 0);
    ~ReportChart();
    void reloadData();

private:
    Ui::ReportChart *ui;
    QSqlQuery *qry;
    QSqlQuery *studentqry;
    QSqlQuery *teacherqry;
    QSqlQueryModel *model;
};

#endif // REPORTCHART_H
