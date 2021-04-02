#include "ray_marched_object.h"

using namespace std;

Hit RayMarchedObject::intersect(Ray const &ray)
{
    double totalDistance = 0.0;
    for (size_t steps = 0; steps < maxSteps; ++steps)
    {
        // March the ray forward.
        Point hit = ray.at(totalDistance);
        double distance = calculateDistance(hit);

        // If we are close enough, we count a hit.
        if (distance < distanceThreshold)
        {
            // Take a step back when calculating the normal.
            Vector normal = calculateNormal(hit - distanceThreshold * ray.D);
            return Hit(totalDistance, normal);
        }

        totalDistance += distance;
    }

    return Hit::NO_HIT();
}

double RayMarchedObject::calculateDistance(Point const &position)
{
    // Apply operations to the input position.
    Point transformed(position);
    transformPosition(transformed);

    // Calculate the distance estimate.
    double distance = distanceEstimator(transformed);

    // Apply operations to the output distance estimate.
    transformDistance(distance);
    return distance;
}

Vector RayMarchedObject::calculateNormal(Point const &hit)
{
    // Small offsets along the coordinate axes.
    Point xOffset(distanceThreshold, 0.0, 0.0);
    Point yOffset(0.0, distanceThreshold, 0.0);
    Point zOffset(0.0, 0.0, distanceThreshold);

    // Calculate the gradient of the distance estimator along these offsets.
    double xGradient = calculateDistance(hit + xOffset) - calculateDistance(hit - xOffset);
    double yGradient = calculateDistance(hit + yOffset) - calculateDistance(hit - yOffset);
    double zGradient = calculateDistance(hit + zOffset) - calculateDistance(hit - zOffset);

    // Approximate the normal by the gradients.
    Vector normal(xGradient, yGradient, zGradient);
    normal.normalize();
    return normal;
}

void RayMarchedObject::transformPosition(Point &position)
{
    for (auto *operation : operations)
        operation->tranformPosition(position);
}

void RayMarchedObject::transformDistance(double &distance)
{
    for (auto *operation : operations)
        operation->tranformDistance(distance);
}
