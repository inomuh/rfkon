#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QtGui>
#include <QMessageBox>

extern int argc;
extern char** argv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::delay(unsigned long ms )
{
    usleep( ms * 1000 );
}

void MainWindow::on_pushButton_Reset_clicked()
{
    d_reset.Parse(reset_json);

    QString text = ui->lineEdit_NodeId->text();
    Value& id = d_reset["NodeID"];

    if (text == "11")
        id.SetString("SensDug11");

    if (text == "12")
        id.SetString("SensDug12");

    if (text == "13")
        id.SetString("SensDug13");

    if (text == "15")
        id.SetString("SensDug15");

    if (text == "16")
        id.SetString("SensDug16");

    if (text == "25")
        id.SetString("SensDug25");

    if (text == "26")
        id.SetString("SensDug26");

    Writer<StringBuffer> writer_reset(buf_reset);
    d_reset.Accept(writer_reset);

    cout << buf_reset.GetString() << endl;

    connector = new TCPConnector();

    //char** av = get_main_argv_temp();
    QStringList arguments = qApp->arguments();

    stream = connector->connect(qPrintable(arguments.at(2)), atoi(qPrintable(arguments.at(1))));

    if(stream)
    {
        message = buf_reset.GetString();
        stream->send(message.c_str(), message.size());
        printf("sent - %s\n", message.c_str());
        len = stream->receive(line, sizeof(line));
        line[len] = 0;
        QMessageBox::information(this, " Response Message from the Node", line);
        printf("received - %s\n", line);
        delete stream;
        buf_reset.Clear();
        delay(100);	// 100ms
    }
    delete connector;
}

void MainWindow::on_pushButton_SetRefreshRate_clicked()
{
    d_refresh_rate.Parse(refresh_rate_json);

    QString text1 = ui->lineEdit_NodeId->text();
    Value& id = d_refresh_rate["NodeID"];

    if (text1 == "11")
        id.SetString("SensDug11");

    if (text1 == "12")
        id.SetString("SensDug12");

    if (text1 == "13")
        id.SetString("SensDug13");

    if (text1 == "15")
        id.SetString("SensDug15");

    if (text1 == "16")
        id.SetString("SensDug16");

    if (text1 == "25")
        id.SetString("SensDug25");

    if (text1 == "26")
        id.SetString("SensDug26");

    QString text2 = ui->lineEdit_RefRate->text();
    Value& refRate = d_refresh_rate["Ref Rate"];

    refRate.SetInt(text2.toInt());

    Writer<StringBuffer> writer_refresh_rate(buf_refresh_rate);
    d_refresh_rate.Accept(writer_refresh_rate);

    cout << buf_refresh_rate.GetString() << endl;

    connector = new TCPConnector();

    //char** av = get_main_argv_temp();
    QStringList arguments = qApp->arguments();

    stream = connector->connect(qPrintable(arguments.at(2)), atoi(qPrintable(arguments.at(1))));

    if (stream) {
        message = buf_refresh_rate.GetString();
        stream->send(message.c_str(), message.size());
        printf("sent - %s\n", message.c_str());
        len = stream->receive(line, sizeof(line));
        QMessageBox::information(this, " Response Message from the Node", line);
        line[len] = 0;
        printf("received - %s\n", line);
        delete stream;
        buf_refresh_rate.Clear();
        delay(100);	// 100ms
    }

    delete connector;

}

void MainWindow::on_pushButton_GetRefreshRate_clicked()
{

    d_refresh_rate.Parse(refresh_rate_json);

    QString text = ui->lineEdit_NodeId->text();
    Value& id = d_refresh_rate["NodeID"];

    if (text == "11")
        id.SetString("SensDug11");

    if (text == "12")
        id.SetString("SensDug12");

    if (text == "13")
        id.SetString("SensDug13");

    if (text == "15")
        id.SetString("SensDug15");

    if (text == "16")
        id.SetString("SensDug16");

    if (text == "25")
        id.SetString("SensDug25");

    if (text == "26")
        id.SetString("SensDug26");

    Value& temp_refresh_rate = d_refresh_rate["Ref Rate"];
    temp_refresh_rate.SetString("-1");

    Writer<StringBuffer> writer_refresh_rate(buf_refresh_rate);
    d_refresh_rate.Accept(writer_refresh_rate);

    cout << buf_refresh_rate.GetString() << endl;

    connector = new TCPConnector();

    //char** av = get_main_argv_temp();
    QStringList arguments = qApp->arguments();

    stream = connector->connect(qPrintable(arguments.at(2)), atoi(qPrintable(arguments.at(1))));

    if (stream) {
        message = buf_refresh_rate.GetString();
        stream->send(message.c_str(), message.size());
        printf("sent - %s\n", message.c_str());
        len = stream->receive(line, sizeof(line));
        QMessageBox::information(this, " Response Message from the Node", line);
        line[len] = 0;
        printf("received - %s\n", line);
        delete stream;
        buf_refresh_rate.Clear();
        delay(100);	// 100ms
    }

    delete connector;
}

void MainWindow::on_pushButton_Alive_clicked()
{
    d_alive.Parse(alive_json);

    QString text = ui->lineEdit_NodeId->text();
    Value& id = d_alive["NodeID"];

    if (text == "11")
        id.SetString("SensDug11");

    if (text == "12")
        id.SetString("SensDug12");

    if (text == "13")
        id.SetString("SensDug13");

    if (text == "15")
        id.SetString("SensDug15");

    if (text == "16")
        id.SetString("SensDug16");

    if (text == "25")
        id.SetString("SensDug25");

    if (text == "26")
        id.SetString("SensDug26");

    Writer<StringBuffer> writer_alive(buf_alive);
    d_alive.Accept(writer_alive);

    cout << buf_alive.GetString() << endl;

    connector = new TCPConnector();

    //char** av = get_main_argv_temp();
    QStringList arguments = qApp->arguments();

    stream = connector->connect(qPrintable(arguments.at(2)), atoi(qPrintable(arguments.at(1))));

    if (stream) {
        message = buf_alive.GetString();
        stream->send(message.c_str(), message.size());
        printf("sent - %s\n", message.c_str());
        len = stream->receive(line, sizeof(line));
        QMessageBox::information(this, " Response Message from the Node", line);
        line[len] = 0;
        printf("received - %s\n", line);
        delete stream;
        buf_alive.Clear();
        delay(100);	// 100ms
    }
    delete connector;

}

void MainWindow::on_pushButton_GetNetStatus_clicked()
{
    d_net_status.Parse(net_status_json);

    QString text = ui->lineEdit_NodeId->text();
    Value& id = d_net_status["NodeID"];

    if (text == "11")
        id.SetString("SensDug11");

    if (text == "12")
        id.SetString("SensDug12");

    if (text == "13")
        id.SetString("SensDug13");

    if (text == "15")
        id.SetString("SensDug15");

    if (text == "16")
        id.SetString("SensDug16");

    if (text == "25")
        id.SetString("SensDug25");

    if (text == "26")
        id.SetString("SensDug26");

    Writer<StringBuffer> writer_net_status(buf_net_status);
    d_net_status.Accept(writer_net_status);

    cout << buf_net_status.GetString() << endl;

    connector = new TCPConnector();

    //char** av = get_main_argv_temp();
    QStringList arguments = qApp->arguments();

    stream = connector->connect(qPrintable(arguments.at(2)), atoi(qPrintable(arguments.at(1))));

    if (stream) {
        message = buf_net_status.GetString();
        stream->send(message.c_str(), message.size());
        printf("sent - %s\n", message.c_str());
        len = stream->receive(line, sizeof(line));
        QMessageBox::information(this, " Response Message from the Node", line);
        line[len] = 0;
        printf("received - %s\n", line);
        delete stream;
        buf_net_status.Clear();
        delay(100);	// 100ms
    }
    delete connector;
}
