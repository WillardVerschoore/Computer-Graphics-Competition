#include "translate.h"

using namespace std;

Translate::Translate(Vector const &translation)
:
    translation(translation)
{}

void Translate::tranformPosition(Point &position)
{
    position -= translation;
}
