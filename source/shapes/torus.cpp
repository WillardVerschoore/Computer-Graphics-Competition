#include "torus.h"

#include <cmath>

using namespace std;

Torus::Torus(double height, double width)
:
    height(height),
    width(width)
{}

double Torus::distanceEstimator(Point const &position)
{
    Point adjusted(position);
    double val1 = sqrt(adjusted.x * adjusted.x + adjusted.z * adjusted.z) - width;
    double val2 = adjusted.y;
    return sqrt(val1 * val1 + val2 * val2) - height;
}
