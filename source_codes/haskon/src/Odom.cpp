#include "Odom.h"

using namespace std;

Odom::Odom( int poseX, int poseY, int poseZ, int roll, int pitch, int yaw )
{
    setPoseX( poseX );
    setPoseY( poseY );
    setPoseZ (poseZ );
    setRoll( roll );
    setPitch( pitch );
    setYaw( yaw );
}

void Odom::setPoseX( int poseX )
{
    this->poseX = poseX;
}
int Odom::getPoseX() const{
    return poseX;
}

void Odom::setPoseY( int poseY )
{
    this->poseY = poseY;
}
int Odom::getPoseY() const{
    return poseY;
}

void Odom::setPoseZ( int poseZ )
{
    this->poseZ = poseZ;
}
int Odom::getPoseZ() const{
    return poseZ;
}

void Odom::setRoll( int roll )
{
    this->roll = roll;
}
int Odom::getRoll() const{
    return roll;
}

void Odom::setPitch( int pitch )
{
    this->pitch = pitch;
}
int Odom::getPitch() const{
    return pitch;
}

void Odom::setYaw( int yaw )
{
    this->yaw = yaw;
}
int Odom::getYaw() const{
    return yaw;
}


// each class requires a public serialize function
void Odom::serialize( JSON::Adapter& adapter )
{
    // this pattern is required
    JSON::Class root( adapter, "odom" );

    // Use the _E variant to able to add more instance
    JSON_E( adapter, poseX );
    JSON_E( adapter, poseY );
    JSON_E( adapter, poseZ );
    JSON_E( adapter, roll );
    JSON_E( adapter, pitch );
    // this is the last member variable we serialize so use the _T variant
    JSON_T( adapter, yaw );
}
