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
    isNodeIdCorrect = false;
    text = ui->lineEdit_NodeId->text();

    d_reset.Parse(reset_json);

    Value& id = d_reset["NodeID"];

    if (text == "11")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug11");
    }
    else if (text == "12")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug12");
    }
    else if (text == "13")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug13");
    }
    else if (text == "15")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug15");
    }
    else if (text == "16")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug16");
    }
    else if (text == "25")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug25");
    }
    else if (text == "26")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug26");
    }
    else
    {
        isNodeIdCorrect = false;
        QMessageBox::warning(this,"Input Error", "Invalid NodeID!");
    }

    if (isNodeIdCorrect)
    {
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
        isNodeIdCorrect = false;
    }
}

void MainWindow::on_pushButton_SetRefreshRate_clicked()
{
    isNodeIdCorrect = false;
    text = ui->lineEdit_NodeId->text();

    d_refresh_rate.Parse(refresh_rate_json);

    Value& id = d_refresh_rate["NodeID"];

    if (text == "11")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug11");
    }
    else if (text == "12")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug12");
    }
    else if (text == "13")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug13");
    }
    else if (text == "15")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug15");
    }
    else if (text == "16")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug16");
    }
    else if (text == "25")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug25");
    }
    else if (text == "26")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug26");
    }
    else
    {
        isNodeIdCorrect = false;
        QMessageBox::warning(this,"Input Error", "Invalid NodeID!");
    }

    refRateText = ui->lineEdit_RefRate->text();
    Value& refRate = d_refresh_rate["Ref Rate"];

    if(refRateText.toInt() >= 0 && refRateText.toInt() < 100000)
    {
        isRefRateCorrect = true;
        refRate.SetInt(refRateText.toInt());
    }
    else
    {
        isRefRateCorrect = false;
        QMessageBox::warning(this,"Input Error", "Invalid Refresh Rate Value!");
    }


    if (isNodeIdCorrect && isRefRateCorrect)
    {

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
        isNodeIdCorrect = false;
        isRefRateCorrect = false;
    }

}

void MainWindow::on_pushButton_GetRefreshRate_clicked()
{
    isNodeIdCorrect = false;
    text = ui->lineEdit_NodeId->text();

    d_refresh_rate.Parse(refresh_rate_json);

    Value& id = d_refresh_rate["NodeID"];

    if (text == "11")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug11");
    }
    else if (text == "12")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug12");
    }
    else if (text == "13")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug13");
    }
    else if (text == "15")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug15");
    }
    else if (text == "16")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug16");
    }
    else if (text == "25")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug25");
    }
    else if (text == "26")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug26");
    }
    else
    {
        isNodeIdCorrect = false;
        QMessageBox::warning(this,"Input Error", "Invalid NodeID!");
    }

    if (isNodeIdCorrect)
    {

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
        isNodeIdCorrect = false;
    }
}

void MainWindow::on_pushButton_Alive_clicked()
{
    isNodeIdCorrect = false;
    text = ui->lineEdit_NodeId->text();

    d_alive.Parse(alive_json);

    Value& id = d_alive["NodeID"];

    if (text == "11")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug11");
    }
    else if (text == "12")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug12");
    }
    else if (text == "13")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug13");
    }
    else if (text == "15")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug15");
    }
    else if (text == "16")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug16");
    }
    else if (text == "25")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug25");
    }
    else if (text == "26")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug26");
    }
    else
    {
        isNodeIdCorrect = false;
        QMessageBox::warning(this,"Input Error", "Invalid NodeID!");
    }

    if (isNodeIdCorrect)
    {

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
        isNodeIdCorrect = false;
    }

}

void MainWindow::on_pushButton_GetNetStatus_clicked()
{
    isNodeIdCorrect = false;
    text = ui->lineEdit_NodeId->text();

    d_net_status.Parse(net_status_json);

    Value& id = d_net_status["NodeID"];

    if (text == "11")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug11");
    }
    else if (text == "12")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug12");
    }
    else if (text == "13")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug13");
    }
    else if (text == "15")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug15");
    }
    else if (text == "16")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug16");
    }
    else if (text == "25")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug25");
    }
    else if (text == "26")
    {
        isNodeIdCorrect = true;
        id.SetString("SensDug26");
    }
    else
    {
        isNodeIdCorrect = false;
        QMessageBox::warning(this,"Input Error", "Invalid NodeID!");
    }

    if (isNodeIdCorrect)
    {

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
        isNodeIdCorrect = false;
    }
}
