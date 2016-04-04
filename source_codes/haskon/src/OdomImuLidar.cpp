#include "OdomImuLidar.h"

OdomImuLidar::OdomImuLidar( int poseX, int poseY, int poseZ, int roll, int pitch, int yaw, int covarianceXy, int covarianceYaw )
{
    setPoseX( poseX );
    setPoseY( poseY );
    setPoseZ( poseZ );
    setRoll( roll );
    setPitch( pitch );
    setYaw( yaw );
    setCovarianceXy( covarianceXy );
    setCovarianceYaw( covarianceYaw );
}

// setters
void OdomImuLidar::setCovarianceXy( int covarianceXy )
{
    this->covarianceXy = covarianceXy;
}

void OdomImuLidar::setCovarianceYaw( int )
{
    this->covarianceYaw = covarianceYaw;
}

// getters
int OdomImuLidar::getCovarianceXy() const
{
    return covarianceXy;
}
int OdomImuLidar::getCovarianceYaw() const
{
    return covarianceYaw;
}


// each class requires a public serialize function
void OdomImuLidar::serialize( JSON::Adapter& adapter )
{
    // this pattern is required
    JSON::Class root( adapter, "odomImuLidar" );

    // Use the _E variant to able to add more instance
    JSON_E( adapter, poseX );
    JSON_E( adapter, poseY );
    JSON_E( adapter, poseZ );
    JSON_E( adapter, roll );
    JSON_E( adapter, pitch );
    JSON_E( adapter, yaw );
    JSON_E( adapter, covarianceXy );
    // this is the last member variable we serialize so use the _T variant
    JSON_T( adapter, covarianceYaw );

}
