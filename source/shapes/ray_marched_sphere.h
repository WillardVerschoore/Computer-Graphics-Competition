#ifndef RAY_MARCHED_SPHERE_H_
#define RAY_MARCHED_SPHERE_H_

#include "../ray_marched_object.h"

class RayMarchedSphere: public RayMarchedObject
{
    public:
        RayMarchedSphere(Point const &center, double radius);

        double distanceEstimator(Point const &position) override;

        Point const center;
        double const radius;
};

#endif
