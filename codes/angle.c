#include "angle.h"
#include <math.h>

double CalcAngle(Vector *v)
{
    return atan2(v->y, v->x);

}
