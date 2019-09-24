#include "reportchart.h"
#include "ui_reportchart.h"
#include <QChartView>
#include <QLineSeries>
#include <QPieSeries>
#include <QBarSeries>
QT_CHARTS_USE_NAMESPACE
ReportChart::ReportChart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReportChart),
    qry(new QSqlQuery),
    model(new QSqlQueryModel),
    studentqry(new QSqlQuery),
    teacherqry(new QSqlQuery)
{
    ui->setupUi(this);
     reloadData();
}

ReportChart::~ReportChart()
{
    delete ui;
    delete  qry;
    delete studentqry;
    delete  teacherqry;
}

void ReportChart::reloadData()
{
    qry->exec("SELECT cate,amount FROM Inventory");
    QPieSeries *pieseries = new QPieSeries();
    double qc=0;
    double yx=0;
    double jx=0;
    double sh=0;
    double sum=0;
    while(qry->next())
    {
        QString m=qry->value(0).toString();
        int amount=qry->value(1).toInt();
        if(m=="青春文学类"){
            qc=amount;
        }else if(m=="营销市场类"){
            yx=amount;
        }else if(m=="计算机类"){
            jx=amount;
        }else{
            sh=amount;
        }
        sum+=amount;
    }

    QString qcname="青春文学类 "+QString::number(qRound(qc*100/sum))+"%";
    QString yxname="营销市场类 "+QString::number(qRound(yx*100/sum))+"%";
    QString jxname="计算机类 "+QString::number(qRound(jx*100/sum))+"%";
    QString shname="社会心理学类 "+QString::number(qRound(sh*100/sum))+"%";
    pieseries->append(qcname, qc);
    pieseries->append(yxname, yx);
    pieseries->append(jxname, jx);
    pieseries->append(shname, sh);
    //pieseries->setLabelsVisible();
    QPieSlice *slice_red = pieseries->slices().at(0);
    QPieSlice *slice_green = pieseries->slices().at(1);
    QPieSlice *slice_blue = pieseries->slices().at(2);
    QPieSlice *slice_black = pieseries->slices().at(3);
    slice_red->setColor(QColor(255,0,0,255));
    slice_green->setColor(QColor(0,255,0,255));
    slice_blue->setColor(QColor(0,0,255,255));
    slice_black->setColor(QColor(0,255,255,255));
    QChart *piechart = new QChart();
    piechart->addSeries(pieseries);
    piechart->setTitle("当前库存图书分类比");
    //piechart->legend()->hide();

    piechart
            ->legend()->setAlignment(Qt::AlignRight);
    ui->graphicsView->setChart(piechart);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);


    QBarSet *set0 = new QBarSet("老师");
    QBarSet *set1 = new QBarSet("学生");

    set0->setBorderColor(QColor(255,128,255));//没什么效果
    //set1->setColor(QColor(255,0,255));//没什么效果
    set1->setLabelColor(QColor(0,0,0));//没什么效果
    int max=0;
    bool flag=teacherqry->exec("select cate,sum(amount) as num from Outbound where type='教师借书' GROUP BY cate");
    qc=0;yx=0;jx=0;sh=0;
    if(flag){
        while(teacherqry->next())
        {
            QString m=teacherqry->value(0).toString();
            int amount=teacherqry->value(1).toInt();
            if(m=="青春文学类"){
                qc=amount;
            }else if(m=="营销市场类"){
                yx=amount;
            }else if(m=="计算机类"){
                jx=amount;
            }else{
                sh=amount;
            }
            if(amount>max)
                max=amount;
        }
    }
    *set0 << qc << yx<< jx <<sh;

    studentqry->exec("select cate,sum(amount) as num from Outbound where type='学生借书' GROUP BY cate");
    qc=0;yx=0;jx=0;sh=0;
    while(studentqry->next())
    {
        QString m=studentqry->value(0).toString();
        int amount=studentqry->value(1).toInt();
        if(m=="青春文学类"){
            qc=amount;
        }else if(m=="营销市场类"){
            yx=amount;
        }else if(m=="计算机类"){
            jx=amount;
        }else{
            sh=amount;
        }
        if(amount>max)
            max=amount;
    }

    *set1 << qc << yx<< jx <<sh;


    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->append(set1);

    series->setVisible(true);//设置条形图是否可视化
    series->setBarWidth(0.8);//设置Bar宽
    series->setLabelsVisible(true);//设置标签是否可视化
    series->setLabelsAngle(30);//设置标签的角度
    //series->setLabelsFormat("@value");//设置标签显示的格式
    //series->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);//设置标签的位置
   // series->setLabelsPrecision(4);//标签中显示的最大有效位数

    //series->setName("BLUE");//当创建多个Bar时才有效


    QChart *chart = new QChart();
    chart->setTitle("图书分类人员借书分析");
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    //chart->setTheme(QChart::ChartThemeBlueCerulean);//设置系统主题
    chart->setAnimationOptions(QChart::AllAnimations);//设置启用或禁用动画
    //chart->setBackgroundBrush(QBrush(QColor(170,170,255)));//设置背景色,主题和背景二选一
    //chart->setDropShadowEnabled(true);//是否背景阴影
    chart->setLocalizeNumbers(true);//数字是否本地化
    //chart->legend()->show();//legend是否显示，show和hide
    chart->createDefaultAxes();//创建默认轴
    //chart->setTitleBrush(QBrush(QColor(255,170,255)));//设置标题Brush
    chart->setTitleFont(QFont("微软雅黑"));//设置标题字体

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignTop);//对齐

    // chart->legend()->setLabelColor(QColor(255,128,255));//设置标签颜色
    chart->legend()->setVisible(true);//设置是否可视

    //chart->legend()->setBorderColor(QColor(255,255,170,185));//设置边框颜色
    QFont font = chart->legend()->font();
    font.setItalic(!font.italic());
    chart->legend()->setFont(font);//设置字体为斜体
    font.setPointSizeF(12);
    chart->legend()->setFont(font);//设置字体大小
    chart->legend()->setFont(QFont("微软雅黑"));//设置字体类型

    QStringList categories;
    categories << "青春文学类" << "营销市场类"<<"计算机类"<<"社会心理学类";
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->setAxisX(axis, series);//设置X轴
    /*X轴*/
    chart->axisX(series)->setVisible(true);
    //chart->axisX(series)->setRange(0,1000);//设置Y轴范围
    //X轴标题
    chart->axisX(series)->setTitleText("图书分类");//设置Y轴标题
    //chart->axisX(series)->setTitleBrush(QBrush(QColor(255,170,255)));
    chart->axisX(series)->setTitleFont(QFont("微软雅黑"));
    chart->axisX(series)->setTitleVisible(true);
    //X轴网格
    //chart->axisX(series)->setGridLineColor(QColor(50,255,50));//设置Y轴网格线颜色
    chart->axisX(series)->setGridLineVisible(true);
    //chart->axisY(series)->setGridLinePen(QPen(QColor(50,255,50)));
    //X轴标签
    chart->axisX(series)->setLabelsAngle(30);
    //chart->axisY(series)->setLabelsBrush(QBrush(QColor(50,255,50)));
    //chart->axisX(series)->setLabelsColor(QColor(255,170,255));
    //chart->axisX(series)->setLabelsFont(QFont("Arial"));
    chart->axisX(series)->setLabelsVisible(true);
    //X轴轴线
    //chart->axisY(series)->setLinePen(QPen(QColor(50,255,50)));
    //chart->axisX(series)->setLinePenColor(QColor(255,170,255));
    chart->axisX(series)->setLineVisible(true);
    chart->axisX(series)->setMax(90);
    chart->axisX(series)->setMin(0);
    //X轴子网格线
    //chart->axisX(series)->setMinorGridLineColor(QColor(255,255,255));
    //chart->axisY(series)->setMinorGridLinePen(QPen(QColor(50,255,50)));
    chart->axisX(series)->setMinorGridLineVisible(true);

    chart->axisX(series)->setReverse(true);//Y轴值反向

    /*Y轴*/
    chart->axisY(series)->setVisible(true);
    chart->axisY(series)->setRange(0,max+max*0.1);//设置Y轴范围
    //Y轴标题
    chart->axisY(series)->setTitleText("图书借书量");//设置Y轴标题
    //chart->axisY(series)->setTitleBrush(QBrush(QColor(255,170,255)));
    chart->axisY(series)->setTitleFont(QFont("微软雅黑"));
    chart->axisY(series)->setTitleVisible(true);
    //Y轴网格
    chart->axisY(series)->setGridLineColor(QColor(50,255,50));//设置Y轴网格线颜色
    chart->axisY(series)->setGridLineVisible(true);

    //chart->axisY(series)->setGridLinePen(QPen(QColor(50,255,50)));
    //Y轴标签
    chart->axisY(series)->setLabelsAngle(10);
    //chart->axisY(series)->setLabelsBrush(QBrush(QColor(50,255,50)));
    //chart->axisY(series)->setLabelsColor(QColor(255,170,255));
    //chart->axisY(series)->setLabelsFont(QFont("Arial"));
    chart->axisY(series)->setLabelsVisible(true);
    //Y轴轴线
    //chart->axisY(series)->setLinePen(QPen(QColor(50,255,50)));
    chart->axisY(series)->setLinePenColor(QColor(255,170,255));
    chart->axisY(series)->setLineVisible(true);
    //最大最小值相当于设置范围
    //chart->axisY(series)->setMax(90);
    //chart->axisY(series)->setMin(10);
    //Y轴子网格线
    chart->axisY(series)->setMinorGridLineColor(QColor(255,255,255));
    //chart->axisY(series)->setMinorGridLinePen(QPen(QColor(50,255,50)));
    chart->axisY(series)->setMinorGridLineVisible(true);
    ui->graphicsViewline->setChart(chart);
    ui->graphicsViewline->setRenderHint(QPainter::Antialiasing);

    /* QLineSeries *series = new QLineSeries();
               series->append(0, 6);
               series->append(2, 4);
               series->append(3, 8);
               series->append(7, 4);
               series->append(10, 5);
               *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

               QChart *chart = new QChart();
               chart->legend()->hide();
               chart->addSeries(series);
               chart->createDefaultAxes();
               chart->setTitle("图书价格");

               ui->graphicsViewline->setChart(chart);
               ui->graphicsViewline->setRenderHint(QPainter::Antialiasing);*/
}
