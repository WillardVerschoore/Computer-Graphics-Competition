#ifndef MANDELBULB_H_
#define MANDELBULB_H_

#include "../ray_marched_object.h"

class Mandelbulb : public RayMarchedObject
{
public:
    Mandelbulb(size_t iterations);

    double distanceEstimator(Point const &position) override;

    size_t const iterations;
};

#endif
