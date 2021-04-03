#include "repeat.h"

#include <cmath>

using namespace std;

Repeat::Repeat(Vector const &period)
:
    period(period)
{}

void Repeat::tranformPosition(Point &position)
{
    // GLSL style floating point modulo function.
    auto mod = [](double x, double y)
    {
        return x - y * floor(x / y);
    };

    if (period.x != 0.0)
        position.x = mod(position.x + 0.5 * period.x, period.x) - 0.5 * period.x;
    if (period.y != 0.0)
        position.y = mod(position.y + 0.5 * period.y, period.y) - 0.5 * period.y;
    if (period.z != 0.0)
        position.z = mod(position.z + 0.5 * period.z, period.z) - 0.5 * period.z;
}
