#include "Wifi.h"

// setters
void Wifi::setSsidAndMac( vector< string > ssidAndMac)
{
    this->ssidAndMac = ssidAndMac;
}

void Wifi::setDbm( vector< int > dbm)
{
    this->dbm = dbm;
}

// getters
vector< string > Wifi::getSsidAndMac() const
{
    return ssidAndMac;
}

vector< int > Wifi::getDbm() const
{
    return dbm;
}



void Wifi::scanWifi()
{
    // Wifi scan results holders
    boost::filesystem::ifstream             fInWifiScan;
    string                                  wifiFileContenHolder;
    vector< string >                        ssidAndMacHolder;
    vector< string >                        ssidHolder;
    vector< string >                        macAddressHolder;
    vector< int >                           signalStrengthHolder;

    // writing the scan into wifi.txt file
    system( "sudo iwlist wlan1 scanning >> wifi.txt" );

    // reading and saving the wifi.txt file into a stream
    fInWifiScan.open( "wifi.txt",ios::in );
    stringstream ssWifi;
    ssWifi << fInWifiScan.rdbuf();

    wifiFileContenHolder = ssWifi.str();

    system( "sudo rm wifi.txt" );

    try
    {
        // regular expresion syntaxes to catch the SSIDs, MAC Addresses and dBms
        boost::regex expSignalStrength( "Signal level=.*?([0-9]+)" );
        boost::regex expMacAddress( "Address: ([0-9A-F:]{17})" );
        boost::regex expSsid( "ESSID:\"([^\"]*)\"");

        // searching for the SSIDs
        boost::match_results< string::const_iterator > whatSsid;
        string::const_iterator startSsid = wifiFileContenHolder.begin();
        string::const_iterator finishSsid = wifiFileContenHolder.end();

        while (boost::regex_search(startSsid, finishSsid, whatSsid, expSsid))
        {
            ssidHolder.push_back( whatSsid[ 1 ] );
            startSsid = whatSsid[ 0 ].second ;
        }


        // searching for MAC Addresses
        boost::match_results< string::const_iterator > whatMacAddress;
        string::const_iterator startMacAddress = wifiFileContenHolder.begin();
        string::const_iterator finishMacAddress = wifiFileContenHolder.end();

        while (boost::regex_search(startMacAddress, finishMacAddress,
                                   whatMacAddress, expMacAddress))
        {
            macAddressHolder.push_back( whatMacAddress[ 1 ] );
            startMacAddress = whatMacAddress[ 0 ].second;
        }

        // searching for dBms
        boost::match_results< string::const_iterator > whatSignalStrength;
        string::const_iterator startSignalStrength = wifiFileContenHolder.begin();
        string::const_iterator finishSignalStrength = wifiFileContenHolder.end();

        while ( boost::regex_search(startSignalStrength, finishSignalStrength,
                                   whatSignalStrength, expSignalStrength) )
        {
            string foundDbm( whatSignalStrength[ 1 ] );
            foundDbm.insert( 0, "-" );
            signalStrengthHolder.push_back( atoi( foundDbm.c_str() ) );
            startSignalStrength = whatSignalStrength[ 0 ].second;
        }

        // saving scan results into the holders
        for(int i = 0; i < ssidHolder.size(); i++)
        {
            cout << "SSID: " << ssidHolder[ i ] << "(" << macAddressHolder[ i ] << ")" << endl;
            ssidAndMacHolder.push_back( ssidHolder[ i ] + "(" + macAddressHolder[ i ] + ")" );
            cout << "dBm: " << signalStrengthHolder[ i ] << endl;

        }
        setSsidAndMac(ssidAndMacHolder);
        setDbm(signalStrengthHolder);

    }
    catch ( boost::bad_expression & ex )
    {
        cout << ex.what();
    }
}

// each class requires a public serialize function
void Wifi::serialize( JSON::Adapter& adapter )
{
    // this pattern is required
    JSON::Class root( adapter, "wifis" );

    // Use the _E variant to able to add more instance
    JSON_E( adapter, ssidAndMac );
    // this is the last member variable we serialize so use the _T variant
    JSON_T( adapter, dbm );
}


