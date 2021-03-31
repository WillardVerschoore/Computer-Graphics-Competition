#ifndef SCALE_H_
#define SCALE_H_

#include "operation.h"

class Scale : public Operation
{
public:
    double scale;

    Scale(double scale);

    void tranformPosition(Point &position) override;
    void tranformDistance(double &distance) override;
};

#endif
