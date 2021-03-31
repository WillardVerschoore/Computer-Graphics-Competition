#ifndef RAY_MARCHED_SPHERE_H_
#define RAY_MARCHED_SPHERE_H_

#include "../ray_marched_object.h"

class RayMarchedSphere: public RayMarchedObject
{
    public:
        double distanceEstimator(Point const &position) override;
};

#endif
