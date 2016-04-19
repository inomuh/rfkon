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
    Odom( double = 0, double = 0, double = 0 , double = 0, double = 0, double = 0 );

    // each class requires a public serialize function
    virtual void serialize( JSON::Adapter& );

    // setters
    void setPoseX( double );
    void setPoseY( double );
    void setPoseZ( double );
    void setRoll( double );
    void setPitch( double );
    void setYaw( double );


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
