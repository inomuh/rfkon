#ifndef WIFI_H_INCLUDED
#define WIFI_H_INCLUDED

class Wifi;

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

#include "json_writer.h"
#include "json_reader.h"
#include "pt.h"

#include <sys/time.h>
#include <fstream>
#include <sstream>
#include <unistd.h>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/regex.hpp>


using namespace std;

class Wifi
{
public:

    // each class requires a public serialize function
    void serialize( JSON::Adapter& );

    // scans the Wifi devices and saves all the info into the class instances
    void scanWifi();

    // setters
    void setSsidAndMac( vector< string > );
    void setDbm( vector< int > );

    // getters
    vector< string > getSsidAndMac() const;
    vector< int > getDbm() const;

private:
    vector< string > ssidAndMac;
    vector< int > dbm;

};


#endif // WIFI_H_INCLUDED
