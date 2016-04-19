#ifndef HASKON_H_INCLUDED
#define HASKON_H_INCLUDED

class Haskon;

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>                       // JSON library</span>
#include <cpprest/uri.h>                        // URI library</span>
#include <cpprest/containerstream.h>            // Async streams backed by STL containers</span>
#include <cpprest/interopstream.h>              // Bridges for integrating Async streams with STL and WinRT streams</span>
#include <cpprest/rawptrstream.h>               // Async streams backed by raw pointer to memory</span>
#include <cpprest/producerconsumerstream.h>     // Async streams for producer consumer scenarios</span>

#include "json_writer.h"
#include "json_reader.h"
#include "pt.h"

#include "Odom.h"
#include "OdomImu.h"
#include "OdomImuLidar.h"
#include "Wifi.h"

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace std;

class Haskon
{
public:
    Haskon();

    // each class requires a public serialize function
    void serialize( JSON::Adapter& );

    // postToKonsens
    pplx::task<void> postToKonsens( string );

    // setters
    void setDeviceId( string );
    void setRefPoint( int );
    void setTestType( string );
    void setWifi( Wifi );
    void setOdom( Odom );
    void setOdomImu( OdomImu );
    void setOdomImuLidar( OdomImuLidar );

    // getters
    string getDeviceId() const;
    int getRefPoint() const;
    string getTestType() const;
    Wifi getWifi() const;
    Odom getOdom() const;
    OdomImu getOdomImu() const;
    OdomImuLidar getOdomImuLidar() const;

private:
    string deviceId;
    int refPoint;
    string testType;
    Odom odom;
    OdomImu odomImu;
    OdomImuLidar odomImuLidar;
    Wifi wifi;
};


#endif // HASKON_H_INCLUDED
