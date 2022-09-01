#include <stdio.h>
#include <math.h>
#include "vector.h"
#include "different_angle.h"
#include "extern_variable.h"

double constant_variable = 3.0;

int main()
{
    Vector v = {constant_variable, 2.0};
    double angle = CalcDiffAngle(&v);
    ShowVector(&v);
    printf("angle=%.1f(pirad)\n", angle / M_PI);
    
    return 0;
}
