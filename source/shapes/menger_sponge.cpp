#include "menger_sponge.h"

#include <cmath>
#include <limits>

using namespace std;

MengerSponge::MengerSponge(size_t iterations)
:
    iterations(iterations)
{}

// Fast Menger Sponge distance estimator adapted from:
// https://www.iquilezles.org/www/articles/menger/menger.htm
double MengerSponge::distanceEstimator(Point const &position)
{
    // GLSL style floating point modulo function.
    auto mod = [](double x, double y)
    {
        return x - y * floor(x / y);
    };

    Point adjusted(position);

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
        r.x = abs(1.0 - 3.0 * abs(r.x));
        r.y = abs(1.0 - 3.0 * abs(r.y));
        r.z = abs(1.0 - 3.0 * abs(r.z));

        double da = max(r.x, r.y);
        double db = max(r.y, r.z);
        double dc = max(r.z, r.x);
        double c = (min(da, min(db, dc)) - 1.0) / scale;

        d = max(d, c);
    }
        
    return d;
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
