#include "mandelbulb.h"

#include <cmath>

using namespace std;

Mandelbulb::Mandelbulb(size_t iterations)
:
    iterations(iterations)
{}

double Mandelbulb::distanceEstimator(Point const &position)
{
    Point w(position);
    double m = w.length_2();
    double dz = 1.0;

    double const bailout = 256.0;
    double const power = 8.0; // Has to be 8.0 because this is used for optimizations.

    for (size_t iteration = 0; iteration < iterations; ++iteration)
    {
        double m2 = m * m;
        double m4 = m2 * m2;
        dz = power * sqrt(m4 * m2 * m) * dz + 1.0;

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

        // dz = power * pow(m,3.5) * dz + 1.0;
        // float r = w.length();
        // float b = power * acos( w.y/r);
        // float a = power * atan2( w.x, w.z );
        // w = position + pow(r, power) * Point(sin(b) * sin(a), cos(b), sin(b) * cos(a));

        m = w.length_2();
        if (m > bailout)
            break;
    }

    return 0.25 * log(m) * sqrt(m) / dz;
}

// double Mandelbulb::distanceEstimator(Point const &position)
// {
//     // TODO: Make these variable passed through the constructor.
//     double const bailout = 5.0;
//     double const power = 8.0;

//     Point adjusted(position);
//     double dr = 1.0;
//     double r = 0.0;

//     for (size_t iteration = 0; iteration < iterations; ++iteration)
//     {
//         r = adjusted.length();
//         if (r > bailout)
//             break;

//         // Convert to polar coordinates.
//         double theta = acos(adjusted.z / r);
//         double phi = atan2(adjusted.y, adjusted.x);
//         dr = pow(r, power - 1.0) * power * dr + 1.0;

//         // Scale and rotate the point.
//         double zr = pow(r, power);
//         theta *= power;
//         phi *= power;
        
//         // Convert back to cartesian coordinates.
//         adjusted = zr * Point(sin(theta) * cos(phi), sin(phi) * sin(theta), cos(theta));
//         adjusted += position;
//     }

//     return 0.5 * log(r) * r / dr;
// }
