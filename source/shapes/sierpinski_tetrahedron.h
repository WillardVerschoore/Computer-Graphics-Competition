#ifndef SIERPINSKI_TETRAHEDRON_H_
#define SIERPINSKI_TETRAHEDRON_H_

#include "../ray_marched_object.h"

class SierpinskiTetrahedron: public RayMarchedObject
{
    public:
        SierpinskiTetrahedron(size_t iterations);

        double distanceEstimator(Point const &position) override;

        size_t const iterations;
};

#endif
