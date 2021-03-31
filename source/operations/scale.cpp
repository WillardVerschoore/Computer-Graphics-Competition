#include "scale.h"

#include <iostream>

using namespace std;

Scale::Scale(double scale)
:
    scale(scale)
{}

void Scale::tranformPosition(Point &position)
{
    position /= scale;
}

void Scale::tranformDistance(double &distance)
{
    distance *= scale;
}
