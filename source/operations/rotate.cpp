#include "rotate.h"

#include <cmath>

using namespace std;

Rotate::Rotate(Vector const &rotation)
:
    rotation(rotation)
{
    // TODO: Is this precomputation actually faster?

    // Compute trigonometric functions.
    double cos_x = cos(rotation.x * M_PI / 180.0);
    double sin_x = sin(rotation.x * M_PI / 180.0);
    double cos_y = cos(rotation.y * M_PI / 180.0);
    double sin_y = sin(rotation.y * M_PI / 180.0);
    double cos_z = cos(rotation.z * M_PI / 180.0);
    double sin_z = sin(rotation.z * M_PI / 180.0);

    // Precompute inverted x rotation matrix;
    xRow1 = Vector{1.0,    0.0,   0.0};
    xRow2 = Vector{0.0,  cos_x, sin_x};
    xRow3 = Vector{0.0, -sin_x, cos_x};

    // Precompute inverted y rotation matrix;
    yRow1 = Vector{cos_y, 0.0, -sin_y};
    yRow2 = Vector{0.0,   1.0,    0.0};
    yRow3 = Vector{sin_y, 0.0,  cos_y};

    // Precompute inverted z rotation matrix;
    zRow1 = Vector{ cos_z, sin_z,  0.0};
    zRow2 = Vector{-sin_z, cos_z,  0.0};
    zRow3 = Vector{   0.0,   0.0,  1.0};
}

void Rotate::tranformPosition(Point &position) 
{
    Point result;

    // Rotate around the z axis. (Multiplication by inverted rotation matrix).
    result.x = position.dot(zRow1);
    result.y = position.dot(zRow2);
    result.z = position.dot(zRow3);
    position = result;

    // Rotate around the y axis. (Multiplication by inverted rotation matrix).
    result.x = position.dot(yRow1);
    result.y = position.dot(yRow2);
    result.z = position.dot(yRow3);
    position = result;

    // Rotate around the x axis. (Multiplication by inverted rotation matrix).
    result.x = position.dot(xRow1);
    result.y = position.dot(xRow2);
    result.z = position.dot(xRow3);
    position = result;
}
