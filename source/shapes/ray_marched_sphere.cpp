#include "ray_marched_sphere.h"

#include <cmath>

RayMarchedSphere::RayMarchedSphere(Point const &center, double radius)
:
    center(center),
    radius(radius)
{}

double RayMarchedSphere::distanceEstimator(Point const &position)
{
    return (position - center).length() - radius;
}
