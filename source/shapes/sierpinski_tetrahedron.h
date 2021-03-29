#ifndef SIERPINSKI_TETRAHEDRON_H_
#define SIERPINSKI_TETRAHEDRON_H_

#include "../ray_marched_object.h"

class SierpinskiTetrahedron: public RayMarchedObject
{
    public:
        SierpinskiTetrahedron(Point const &center, double size, size_t iterations);

        double distanceEstimator(Point const &position) override;

        Point const center;
        double const size;
        size_t const iterations;
};

#endif
