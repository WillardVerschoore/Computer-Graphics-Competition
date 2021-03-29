#include "torus.h"

#include <cmath>

using namespace std;

Torus::Torus(Point const &center, double height, double width)
:
    center(center),
    height(height),
    width(width)
{}

double Torus::distanceEstimator(Point const &position)
{
    Point adjusted = position - center;
    double val1 = sqrt(adjusted.x * adjusted.x + adjusted.z * adjusted.z) - width;
    double val2 = adjusted.y;
    return sqrt(val1 * val1 + val2 * val2) - height;
}
