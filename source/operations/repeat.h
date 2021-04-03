#ifndef REPEAT_H_
#define REPEAT_H_

#include "operation.h"

class Repeat : public Operation
{
public:
    Vector period;

    Repeat(Vector const &period);

    void tranformPosition(Point &position) override;
};

#endif
