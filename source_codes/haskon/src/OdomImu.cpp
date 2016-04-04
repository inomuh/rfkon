#include "OdomImu.h"

OdomImu::OdomImu( int poseX, int poseY, int poseZ, int roll, int pitch, int yaw )
{
    setPoseX( poseX );
    setPoseY( poseY );
    setPoseZ( poseZ );
    setRoll( roll );
    setPitch( pitch );
    setYaw( yaw );
}

// each class requires a public serialize function
void OdomImu::serialize( JSON::Adapter& adapter )
{
    // this pattern is required
    JSON::Class root( adapter, "odomImu" );

    // Use the _E variant to able to add more instance
    JSON_E( adapter, poseX );
    JSON_E( adapter, poseY );
    JSON_E( adapter, poseZ );
    JSON_E( adapter, roll );
    JSON_E( adapter, pitch );
    // this is the last member variable we serialize so use the _T variant
    JSON_T( adapter, yaw );
}
