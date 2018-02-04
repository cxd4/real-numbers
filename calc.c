#include <float.h>
#include <signal.h>

#include "strtor.h"
#include <stdio.h>

#include "calc.h"
#include <math.h>

int add(int argc, char* argv[])
{
    real answer, source;
    register int i;

    answer = 0;
    if (argc < 3)
        return -1;
    for (i = 1; i < argc; i++) {
        source = strtor(argv[i]);
        answer += source;
    }
    printf("%g\n", answer);
    return 0;
}
int subtract(int argc, char* argv[])
{
    real answer, source;
    register int i;

    if (argc < 3)
        return -1;
    answer = strtor(argv[1]);
    for (i = 2; i < argc; i++) {
        source = strtor(argv[i]);
        answer -= source;
    }
    printf("%g\n", answer);
    return 0;
}
int multiply(int argc, char* argv[])
{
    real answer, source;
    register int i;

    answer = 1;
    if (argc < 3)
        return -1;
    for (i = 1; i < argc; i++) {
        source = strtor(argv[i]);
        answer *= source;
    }
    printf("%g\n", answer);
    return 0;
}
int divide(int argc, char* argv[])
{
    real answer, source;
    register int i;

    if (argc < 3)
        return -1;
    answer = strtor(argv[1]);
    for (i = 2; i < argc; i++) {
        source = strtor(argv[i]);
#ifndef SIGFPE
        if (source == 0)
            source = +(DBL_MIN);
#endif
        answer /= source;
    }
    printf("%g\n", answer);
    return 0;
}

const math_operation op_functions[] = {
     add     , iadd     , wadd     ,
     subtract, isubtract, wsubtract,
     multiply, imultiply, wmultiply,
     divide  , idivide  , wdivide  ,
};
