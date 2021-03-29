#include "ray_marched_object.h"

using namespace std;

Hit RayMarchedObject::intersect(Ray const &ray)
{
    double totalDistance = 0.0;
    for (size_t steps = 0; steps < maxSteps; ++steps)
    {
        Point hit = ray.at(totalDistance);
        double distance = distanceEstimator(hit);

        // If we are close enough, we count a hit.
        if (distance < distanceThreshold)
        {
            Vector normal = calculateNormal(hit);
            return Hit(totalDistance, normal);
        }

        totalDistance += distance;
    }

    return Hit::NO_HIT();
}

Vector RayMarchedObject::calculateNormal(Point const &hit)
{
    // Small offsets along the coordinate axes. TODO: Use distanceThreshold or new variable for offset size.
    Point xOffset(1E-3, 0.0, 0.0);
    Point yOffset(0.0, 1E-3, 0.0);
    Point zOffset(0.0, 0.0, 1E-3);

    // Calculate the gradient of the distance estimator along these offsets.
    double xGradient = distanceEstimator(hit + xOffset) - distanceEstimator(hit - xOffset);
    double yGradient = distanceEstimator(hit + yOffset) - distanceEstimator(hit - yOffset);
    double zGradient = distanceEstimator(hit + zOffset) - distanceEstimator(hit - zOffset);

    // Approximate the normal by the gradients.
    Vector normal(xGradient, yGradient, zGradient);
    normal.normalize();
    return normal;
}
