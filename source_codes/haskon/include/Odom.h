#ifndef ODOM_H_INCLUDED
#define ODOM_H_INCLUDED

class Odom;

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

#include "json_writer.h"
#include "json_reader.h"
#include "pt.h"

using namespace std;

class Odom
{
public:
    Odom( int = 0, int = 0, int = 0 , int = 0, int = 0, int = 0 );

    // each class requires a public serialize function
    virtual void serialize( JSON::Adapter& );

    // setters
    void setPoseX( int );
    void setPoseY( int );
    void setPoseZ( int );
    void setRoll( int );
    void setPitch( int );
    void setYaw( int );


    // getters
    int getPoseX() const;
    int getPoseY() const;
    int getPoseZ() const;
    int getRoll() const;
    int getPitch() const;
    int getYaw() const;


protected:
    int poseX;
    int poseY;
    int poseZ;
    int roll;
    int pitch;
    int yaw;

};


#endif // ODOM_H_INCLUDED
