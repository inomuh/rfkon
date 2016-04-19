#include "OdomImuLidar.h"

OdomImuLidar::OdomImuLidar( double poseX, double poseY, double poseZ, double roll, double pitch, double yaw, double covarianceXy, double covarianceYaw )
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
void OdomImuLidar::setCovarianceXy( double covarianceXy )
{
    this->covarianceXy = covarianceXy;
}

void OdomImuLidar::setCovarianceYaw( double covarianceYaw )
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
