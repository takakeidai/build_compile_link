#include "angle.h"
#include <math.h>

double CalcDiffAngle(Vector *v)
{   double num;
    num = 1 - atan2(v->y, v->x);
    return num;

}
