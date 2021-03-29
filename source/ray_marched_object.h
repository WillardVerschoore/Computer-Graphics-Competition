#ifndef RAY_MARCHED_OBJECT_H_
#define RAY_MARCHED_OBJECT_H_

#include "object.h"

class RayMarchedObject : public Object
{
public:
    size_t maxSteps = 128;
    double distanceThreshold = 1E-3;

    Hit intersect(Ray const &ray) override;
    virtual double distanceEstimator(Point const &position) = 0;

private:
    Vector calculateNormal(Point const &hit);
};

#endif
