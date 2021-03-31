#ifndef OPERATION_H_
#define OPERATION_H_

#include "../triple.h"

#include <iostream>

class Operation
{
public:
    virtual void tranformPosition(Point &position);
    virtual void tranformDistance(double &distance);
};

#endif
