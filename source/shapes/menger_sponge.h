#ifndef MENGER_SPONGE_H_
#define MENGER_SPONGE_H_

#include "../ray_marched_object.h"

class MengerSponge : public RayMarchedObject
{
public:
    MengerSponge(size_t iterations);

    double distanceEstimator(Point const &position) override;

    size_t const iterations;

private:
    double box(Point const &position, Point const &b);
    double cross(Point const &position);
};

#endif
