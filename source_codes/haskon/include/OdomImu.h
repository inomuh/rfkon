#ifndef ODOMIMU_H_INCLUDED
#define ODOMIMU_H_INCLUDED

#include "Odom.h"

class OdomImu : public Odom
{
public:
    OdomImu( double = 0, double = 0, double = 0 , double = 0, double = 0, double = 0 );

    // each class requires a public serialize function
    virtual void serialize( JSON::Adapter& );

};



#endif // ODOMIMU_H_INCLUDED
