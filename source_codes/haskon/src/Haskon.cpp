#include "Haskon.h"

Haskon::Haskon()
{
    setDeviceId( "evarobot" ) ;
    setRefPoint( 0 );
    setTestType( "Robot Mode" );
}


void Haskon::setDeviceId( string deviceId )
{
    this->deviceId = deviceId;
}
std::string Haskon::getDeviceId() const
{
    return deviceId;
}
void Haskon::setRefPoint( int refPoint )
{
    this->refPoint = refPoint;
}

void Haskon::setTestType( string testType )
{
    this->testType = testType;
}
int Haskon::getRefPoint() const
{
    return refPoint;
}
string Haskon::getTestType() const
{
    return testType;
}
void Haskon::setWifi( Wifi wifi )
{
    this->wifi = wifi;
}
Wifi Haskon::getWifi() const
{
    return wifi;
}
void Haskon::setOdom( Odom odom )
{
    this->odom = odom;
}
Odom Haskon::getOdom() const
{
    return odom;
}
void Haskon::setOdomImu( OdomImu odomImu )
{
    this->odomImu = odomImu;
}
OdomImu Haskon::getOdomImu() const
{
    return odomImu;
}
void Haskon::setOdomImuLidar( OdomImuLidar odomImuLidar )
{
    this->odomImuLidar = odomImuLidar;
}
OdomImuLidar Haskon::getOdomImuLidar() const
{
    return odomImuLidar;
}

// 'postToKonsens' function is created to send the HasKon data collected from evarobot
// to the KonSens through Web Service.
//
// Instance of the Haskon:
//      string deviceId;
//      int refPoint;
//      int battery;
//      string testType;
//      Odom odom;
//      OdomImu odomImu;
//      OdomImuLidar odomImuLidar;
//      Wifi wifi;
//
// Default values of the instances:
//      deviceId = "evarobot";
//      battery = -1;
//      refPoint = 0;
//      testType = "Robot Mode";
//
// Note: You don't have to change default values of deviceId, battery, testType. However,
// just remember to set the refPoint based on the evarobot's position.
pplx::task<void> Haskon::postToKonsens( string jsonString )
{
    return pplx::create_task( [ jsonString ]
    {
        cout << "JSON data to send -> " << jsonString << endl;

        // parsing JSON string which contains the Haskon Class instance information
        json::value casablancaObject = json::value::parse( jsonString );

        // creating http_client to send the request.
        // REMINDER:
        // inonet server ip = http://192.168.4.1:8080
        // RFKON-Mesh server ip = http://192.168.3.3:8080
        http_client client( U( "http://192.168.4.1:8080/" ) );

        // building request URI and start the request.
        uri_builder builder( U( "/RfkonWebService/GezkonDataFetcher/postHaskon" ) );

        // creating a POST request and configuring with the URI and the haskonCasablancaObject
        http_request request( methods::POST );
        request.set_request_uri( builder.to_string() );
        request.set_body( casablancaObject );

        // sending the request to the KonSens
        return client.request( request );
    })
            //Handling the response message
            .then( []( http_response response )
    {
        if( response.status_code() == status_codes::OK )
        {
            cout << "Received response status code: " << response.status_code() << endl;
        }
        else
        {
            cout << "Received response status code: " << response.status_code() << endl;
            cout << "Something went wrong with the Web Service request" << endl;
        }


    });
}


// each class requires a public serialize function
void Haskon::serialize( JSON::Adapter& adapter )
{
    // this pattern is required
    JSON::Class root( adapter, "haskon" );
    // Use the _E variant to able to add more instance
    JSON_E( adapter, deviceId );
    JSON_E( adapter, refPoint );
    JSON_E( adapter, testType );
    JSON_E( adapter, wifi );
    JSON_E( adapter, odom );
    JSON_E( adapter, odomImu );
    // this is the last member variable we serialize so use the _T variant
    JSON_T( adapter, odomImuLidar );

}
