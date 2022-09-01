#include "angle_related.h"
#include <math.h>

double CalcAngle(Vector *v)
{   double num;
    num = 1 - atan2(v->y, v->x);
    return num;
}
