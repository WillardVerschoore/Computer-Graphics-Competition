#include "ray_marched_sphere.h"

#include <cmath>

RayMarchedSphere::RayMarchedSphere(Point const &center, double radius)
:
    center(center),
    radius(radius)
{}

Hit RayMarchedSphere::intersect(Ray const &ray)
{
    // TODO: Variable for max steps.
    // TODO: Variable for distance threshold.

    double totalDistance = 0.0;
    for (size_t steps = 0; steps < 64; ++steps)
    {
        // March the distance estimate along the ray direction.
        Point hit = ray.at(totalDistance);
        double distance = distanceEstimator(hit);
        totalDistance += distance;

        // If we are close enough, we count a hit.
        if (distance < 1E-3)
        {
            hit = ray.at(totalDistance);
            Vector normal = (hit - center).normalized(); // Just use standard sphere normal for now.
            return Hit(totalDistance, normal);
        }
    }

    return Hit::NO_HIT();
}

double RayMarchedSphere::distanceEstimator(Point const &position)
{
    return (position - center).length() - radius;
}
