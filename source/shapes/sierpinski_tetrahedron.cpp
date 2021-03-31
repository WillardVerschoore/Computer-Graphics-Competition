#include "sierpinski_tetrahedron.h"

#include <cmath>
#include <iostream>

using namespace std;

SierpinskiTetrahedron::SierpinskiTetrahedron(size_t iterations)
:
    iterations(iterations)
{}

double SierpinskiTetrahedron::distanceEstimator(Point const &position)
{
    Point adjusted(position);

    Point vertex1( 1.0,  1.0,  1.0);
    Point vertex2(-1.0, -1.0,  1.0);
    Point vertex3( 1.0, -1.0, -1.0);
    Point vertex4(-1.0,  1.0, -1.0);

    Point closest;
    double distance, closestDistance;

    for (size_t index = 0; index < iterations; ++index)
    {
        closest = vertex1;
        closestDistance = (adjusted - vertex1).length();

        distance = (adjusted - vertex2).length();
        if (distance < closestDistance)
        {
            closest = vertex2;
            closestDistance = distance;
        }

        distance = (adjusted - vertex3).length();
        if (distance < closestDistance)
        {
            closest = vertex3;
            closestDistance = distance;
        }

        distance = (adjusted - vertex4).length();
        if (distance < closestDistance)
        {
            closest = vertex4;
            closestDistance = distance;
        }

        adjusted = 2.0 * adjusted - closest * (2.0 - 1.0);
    }

    return adjusted.length() * pow(2.0, -static_cast<double>(iterations));
}
