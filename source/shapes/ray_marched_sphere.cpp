#include "ray_marched_sphere.h"

#include <cmath>

double RayMarchedSphere::distanceEstimator(Point const &position)
{
    return position.length() - 1.0; // Default radius is 1, this can be changed using a scale operation.
}
