#include "octahedron.h"

#include <algorithm>
#include <cmath>

using namespace std;

Octahedron::Octahedron(Point const &position, double size)
:
    position(position),
    size(size)
{}

double Octahedron::distanceEstimator(Point const &position)
{
    Point adjusted = (position - this->position) / size;
    adjusted.x = abs(adjusted.x);
    adjusted.y = abs(adjusted.y);
    adjusted.z = abs(adjusted.z);

    double m = adjusted.x + adjusted.y + adjusted.z - 1.0;
    Point q;

    if (3.0 * adjusted.x < m)
    {
        q = adjusted;
    }
    else if (3.0 * adjusted.y < m)
    {
        q.x = adjusted.y;
        q.y = adjusted.z;
        q.z = adjusted.x;
    }
    else if (3.0 * adjusted.z < m)
    {
        q.x = adjusted.z;
        q.y = adjusted.x;
        q.z = adjusted.y;
    }
    else
    {
        return m * 0.57735027 * size;
    }

    double k = clamp(0.5 * (q.z - q.y + 1.0), 0.0 , 1.0);
    return Point{q.x, q.y - 1.0 + k, q.z - k}.length() * size;
}
