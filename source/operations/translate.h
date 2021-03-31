#ifndef TRANSLATE_H_
#define TRANSLATE_H_

#include "operation.h"

class Translate : public Operation
{
public:
    Vector translation;

    Translate(Vector const &translation);

    void tranformPosition(Point &position) override;
};

#endif
