#include "mandelbulb.h"

#include <cmath>

using namespace std;

Mandelbulb::Mandelbulb(size_t iterations)
:
    iterations(iterations)
{}

// Fast (on the CPU atleast) Mandelbulb distance estimator adapted from:
// https://www.iquilezles.org/www/articles/mandelbulb/mandelbulb.htm
double Mandelbulb::distanceEstimator(Point const &position)
{
    Point w(position);
    double m = w.length_2();
    double dz = 1.0;

    double const bailout = 256.0;

    for (size_t iteration = 0; iteration < iterations; ++iteration)
    {
        double m2 = m * m;
        double m4 = m2 * m2;
        dz = 8.0 * sqrt(m4 * m2 * m) * dz + 1.0;

        double x = w.x; double x2 = x * x; double x4 = x2 * x2;
        double y = w.y; double y2 = y * y; double y4 = y2 * y2;
        double z = w.z; double z2 = z * z; double z4 = z2 * z2;

        double k3 = x2 + z2;
        double k2 = 1.0 / sqrt(k3 * k3 * k3 * k3 * k3 * k3 * k3);
        double k1 = x4 + y4 + z4 - 6.0 * y2 * z2 - 6.0 * x2 * y2 + 2.0 * z2 * x2;
        double k4 = x2 - y2 + z2;

        w.x = position.x + 64.0 * x * y * z * (x2 - z2) * k4 * (x4 - 6.0 * x2 * z2 + z4) * k1 * k2;
        w.y = position.y + -16.0 * y2 * k3 * k4 * k4 + k1 * k1;
        w.z = position.z + -8.0 * y * k4 * (x4 * x4 - 28.0 * x4 * x2 * z2 + 70.0 * x4 * z4 - 28.0 * x2 * z2 * z4 + z4 * z4) * k1 * k2;

        m = w.length_2();
        if (m > bailout)
            break;
    }

    return 0.25 * log(m) * sqrt(m) / dz;
}
