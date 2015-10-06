#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "tcpconnector.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <unistd.h>

using namespace std;
using namespace rapidjson;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    inline void delay( unsigned long);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_Reset_clicked();

    void on_pushButton_SetRefreshRate_clicked();

    void on_pushButton_GetRefreshRate_clicked();

    void on_pushButton_Alive_clicked();

    void on_pushButton_GetNetStatus_clicked();

private:
    Ui::MainWindow *ui;

    char* reset_json = "{\"Reset\":false,\"NodeID\":\"0\"}";
    char* refresh_rate_json = "{\"Ref Rate\":\"10\",\"NodeID\":\"0\", \"Status\":false}";
    char* alive_json  = "{\"Alive\":false,\"NodeID\":\"0\"}";
    char* net_status_json = "{\"IP\":\"0\",\"Subnet\":\"0\", \"Gateway\":\"0\",\"DNS\":\"0\",\"Name\":\"0\",\"NodeID\":\"0\",\"Status\":false}";
    Document d_reset, d_refresh_rate, d_alive, d_net_status;
    StringBuffer buf_reset, buf_refresh_rate, buf_alive, buf_net_status;

    int len;
    string message;
    char line[256];
    TCPConnector* connector;
    TCPStream* stream;
    bool isNodeIdCorrect, isRefRateCorrect;
    QString text, refRateText;
};

#endif // MAINWINDOW_H
