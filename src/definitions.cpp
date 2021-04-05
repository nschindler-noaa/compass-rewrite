
#include "definitions.h"


bool floatIsEqual(float val1, float val2)
{
    bool equal = true;
    if (val1 < val2-PRECISION)
        equal = false;
    else if (val1 > val2+PRECISION)
        equal = false;

    return equal;
}

bool floatIsNotEqual(float val1, float val2)
{
    bool equal = floatIsEqual(val1, val2);

    return !equal;
}

bool doubleIsEqual(double val1, double val2)
{
    bool equal = true;
    double Precision = PRECISION * .0001;
    if (val1 < val2-Precision)
        equal = false;
    else if (val1 > val2+Precision)
        equal = false;

    return equal;
}
