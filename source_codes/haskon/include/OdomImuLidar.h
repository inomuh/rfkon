#ifndef ODOMIMULIDAR_H_INCLUDED
#define ODOMIMULIDAR_H_INCLUDED

#include "Odom.h"

class OdomImuLidar : public Odom
{
public:
    OdomImuLidar( int = 0, int = 0, int = 0 , int = 0, int = 0, int = 0 , int = 0, int = 0 );

    // each class requires a public serialize function
    virtual void serialize( JSON::Adapter& );

    // setters
    void setCovarianceXy( int );
    void setCovarianceYaw( int );

    // getters
    int getCovarianceXy() const;
    int getCovarianceYaw() const;

private:
    int covarianceXy;
    int covarianceYaw;

};



#endif // ODOMIMULIDAR_H_INCLUDED
