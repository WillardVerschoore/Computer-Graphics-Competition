#ifndef OCTAHEDRON_H_
#define OCTAHEDRON_H_

#include "../ray_marched_object.h"

class Octahedron : public RayMarchedObject
{
public:
    double distanceEstimator(Point const &position) override;
};

#endif
