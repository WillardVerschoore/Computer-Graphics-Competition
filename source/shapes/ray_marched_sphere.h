#ifndef RAY_MARCHED_SPHERE_H_
#define RAY_MARCHED_SPHERE_H_

#include "../object.h"

class RayMarchedSphere: public Object
{
    public:
        RayMarchedSphere(Point const &center, double radius);

        Hit intersect(Ray const &ray) override;
        double distanceEstimator(Point const &position);

        Point const center;
        double const radius;
};

#endif
