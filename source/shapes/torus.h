#ifndef TORUS_H_
#define TORUS_H_

#include "../ray_marched_object.h"

class Torus: public RayMarchedObject
{
    public:
        Torus(Point const &center, double height, double width);

        double distanceEstimator(Point const &position) override;

        Point const center;
        double const height;
        double const width;
};

#endif
