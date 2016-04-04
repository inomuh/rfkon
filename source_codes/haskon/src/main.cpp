#include "Haskon.h"

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

using namespace std;


int main(int argc, char* argv[])
{
    Haskon haskon;
    Odom odom( 1, 1, 1, 1, 1, 1 );
    OdomImu odomImu ( 5, 5, 5, 5, 5, 5);
    OdomImuLidar odomImuLidar( 9, 9, 9, 9, 9, 9, 9, 9 );
    Wifi wifi;
    wifi.scanWifi();

    haskon.setRefPoint( -1 );
    haskon.setWifi( wifi );
    haskon.setOdom( odom );
    haskon.setOdomImu( odomImu );
    haskon.setOdomImuLidar( odomImuLidar );

    // converting haskon object into JSON formatted string
    string haskonJsonString = JSON::producer< Haskon >::convert( haskon );
    // passing the haskon json string to the Web Service handler
    haskon.postToKonsens( haskonJsonString ).wait();

    return 0;
}
