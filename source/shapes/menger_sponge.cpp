#include "menger_sponge.h"

#include <cmath>
#include <limits>

using namespace std;

MengerSponge::MengerSponge(Point const &position, double size, size_t iterations)
:
    position(position),
    size(size),
    iterations(iterations)
{}

double MengerSponge::distanceEstimator(Point const &position)
{
    auto mod = [](double x, double y)
    {
        return x - y * floor(x / y);
    };

    Point adjusted = (position - this->position) / size;

    double d = box(adjusted, Point{1.0, 1.0, 1.0});

    double scale = 1.0;
    for (size_t iteration = 0; iteration < iterations; ++iteration)
    {
        Point a = adjusted * scale;
        a.x = mod(a.x, 2.0) - 1.0;
        a.y = mod(a.y, 2.0) - 1.0;
        a.z = mod(a.z, 2.0) - 1.0;

        scale *= 3.0;
        
        Point r =  a;
        r.x = 1.0 - 3.0 * abs(r.x);
        r.y = 1.0 - 3.0 * abs(r.y);
        r.z = 1.0 - 3.0 * abs(r.z);

        double c = cross(r) / scale;
        d = max(d, c);
    }
    
    return d * size;
}

double MengerSponge::box(Point const &position, Point const &b)
{
    Point adjusted = position;
    adjusted.x = abs(adjusted.x);
    adjusted.y = abs(adjusted.y);
    adjusted.z = abs(adjusted.z);
    adjusted -= b;

    double m = min(max(adjusted.x, max(adjusted.y, adjusted.z)), 0.0);
    adjusted.x = max(adjusted.x, 0.0);
    adjusted.y = max(adjusted.y, 0.0);
    adjusted.z = max(adjusted.z, 0.0);
    return adjusted.length() + m;
}

double MengerSponge::cross(Point const &position)
{
    double inf = numeric_limits<double>::infinity();

    Point adjusted = position;
    double da = box(adjusted, Point{inf, 1.0, 1.0});

    adjusted.x = position.y;
    adjusted.y = position.z;
    adjusted.z = position.x;
    double db = box(adjusted, Point{1.0, inf, 1.0});

    adjusted.x = position.z;
    adjusted.y = position.x;
    adjusted.z = position.y;
    double dc = box(adjusted, Point{1.0, 1.0, inf});

    return min(da, min(db, dc));
}
