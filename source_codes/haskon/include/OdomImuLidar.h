#ifndef ODOMIMULIDAR_H_INCLUDED
#define ODOMIMULIDAR_H_INCLUDED

#include "Odom.h"

class OdomImuLidar : public Odom
{
public:
    OdomImuLidar( double = 0, double = 0, double = 0 , double = 0, double = 0, double = 0 , double = 0, double = 0 );

    // each class requires a public serialize function
    virtual void serialize( JSON::Adapter& );

    // setters
    void setCovarianceXy( double );
    void setCovarianceYaw( double );

    // getters
    int getCovarianceXy() const;
    int getCovarianceYaw() const;

private:
    double covarianceXy;
    double covarianceYaw;

};



#endif // ODOMIMULIDAR_H_INCLUDED
