#ifndef RAY_MARCHED_OBJECT_H_
#define RAY_MARCHED_OBJECT_H_

#include "object.h"
#include "operations/operation.h"

#include <vector>

class RayMarchedObject : public Object
{
public:
    size_t maxSteps = 128;
    double distanceThreshold = 1E-3;
    std::vector<Operation*> operations;

    Hit intersect(Ray const &ray) override;
    virtual double distanceEstimator(Point const &position) = 0;

private:
    double calculateDistance(Point const &position);
    Vector calculateNormal(Point const &hit);

    void transformPosition(Point &position);
    void transformDistance(double &distance);
};

#endif
