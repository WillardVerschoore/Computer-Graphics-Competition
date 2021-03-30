#ifndef OCTAHEDRON_H_
#define OCTAHEDRON_H_

#include "../ray_marched_object.h"

class Octahedron : public RayMarchedObject
{
public:
    Octahedron(Point const &position, double size);

    double distanceEstimator(Point const &position) override;

    Point const position;
    double const size;
};

#endif
