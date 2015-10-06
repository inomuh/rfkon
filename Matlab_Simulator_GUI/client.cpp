/*
   client.cpp

   Test client for the tcpsockets classes.

   ------------------------------------------

   Copyright � 2013 [Vic Hargrave - http://vichargrave.com]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "tcpconnector.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>

using namespace std;
using namespace rapidjson;

#include <unistd.h>

inline void delay( unsigned long ms )
{
    usleep( ms * 1000 );
}

int main(int argc, char** argv)
{


    // 1. Parse a JSON string into DOM.


    d_reset.Parse(reset_json);
    d_refresh_rate.Parse(refresh_rate_json);
    d_alive.Parse(alive_json);
    d_net_status.Parse(net_status_json);


    //    // 2. Modify it by DOM.
    //    Value& s = d["stars"];
    //    s.SetInt(s.GetInt() + 1);

    // 3. Stringify the DOM
    StringBuffer buf_reset, buf_refresh_rate, buf_alive, buf_net_status;

    Writer<StringBuffer> writer_reset(buf_reset);
    d_reset.Accept(writer_reset);

    Writer<StringBuffer> writer_refresh_rate(buf_refresh_rate);
    d_refresh_rate.Accept(writer_refresh_rate);

    Writer<StringBuffer> writer_alive(buf_alive);
    d_alive.Accept(writer_alive);

    Writer<StringBuffer> writer_net_status(buf_net_status);
    d_net_status.Accept(writer_net_status);

    // Output the ready message
    cout << "Messages that are ready to send: " << endl;
    cout << buf_reset.GetString() << std::endl;
    cout << buf_refresh_rate.GetString() << std::endl;
    cout << buf_alive.GetString() << std::endl;
    cout << buf_net_status.GetString() << std::endl;

    if (argc != 3) {
        printf("usage: %s <port> <ip>\n", argv[0]);
        exit(1);
    }

    int len;
    string message;
    char line[256];
    TCPConnector* connector = new TCPConnector();
    TCPStream* stream = connector->connect(argv[2], atoi(argv[1]));
    if(stream)
    {
        message = buf_reset.GetString();
        stream->send(message.c_str(), message.size());
        printf("sent - %s\n", message.c_str());
        len = stream->receive(line, sizeof(line));
        line[len] = 0;
        printf("received - %s\n", line);

        delete stream;
        delay(1000);	// 1 seconds
    }


    stream = connector->connect(argv[2], atoi(argv[1]));
    if (stream) {
        message = buf_refresh_rate.GetString();
        stream->send(message.c_str(), message.size());
        printf("sent - %s\n", message.c_str());
        len = stream->receive(line, sizeof(line));
        line[len] = 0;
        printf("received - %s\n", line);
        delete stream;
    }

    stream = connector->connect(argv[2], atoi(argv[1]));
    if (stream) {
        message = buf_alive.GetString();
        stream->send(message.c_str(), message.size());
        printf("sent - %s\n", message.c_str());
        len = stream->receive(line, sizeof(line));
        line[len] = 0;
        printf("received - %s\n", line);
        delete stream;
    }


    stream = connector->connect(argv[2], atoi(argv[1]));
    if (stream) {
        message = buf_net_status.GetString();
        stream->send(message.c_str(), message.size());
        printf("sent - %s\n", message.c_str());
        len = stream->receive(line, sizeof(line));
        line[len] = 0;
        printf("received - %s\n", line);
        delete stream;
    }




    exit(0);
}
