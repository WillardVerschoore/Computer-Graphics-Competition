#ifndef ROTATE_H_
#define ROTATE_H_

#include "operation.h"

class Rotate : public Operation
{
public:
    Vector rotation;

    Rotate(Vector const &rotation);

    void tranformPosition(Point &position) override;

private:
    Vector xRow1, xRow2, xRow3;
    Vector yRow1, yRow2, yRow3;
    Vector zRow1, zRow2, zRow3;
};

#endif
