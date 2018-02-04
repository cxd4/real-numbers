#include <float.h>
#include <signal.h>

#include "calc.h"
#include <math.h>

#include "strtor.h"
real r_result;

int radd(int argc, char* argv[])
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
    r_result = answer;
    return 0;
}
int rsubtract(int argc, char* argv[])
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
    r_result = answer;
    return 0;
}
int rmultiply(int argc, char* argv[])
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
    r_result = answer;
    return 0;
}
int rdivide(int argc, char* argv[])
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
    r_result = answer;
    return 0;
}
int rmodulo(int argc, char* argv[])
{
    real answer, source, frac;
    register int i;

    if (argc < 3)
        return -1;
    answer = strtor(argv[1]);
    for (i = 2; i < argc; i++) {
        source = strtor(argv[i]);
#ifndef SIGFPE
        if (source == 0)
            return 1;
#endif
        answer = fmod(answer, source);
    }
    r_result = answer;
    return 0;
}

const math_operation op_functions[] = {
    radd     , iadd     , wadd     ,
    rsubtract, isubtract, wsubtract,
    rmultiply, imultiply, wmultiply,
    rdivide  , idivide  , wdivide  ,
    rmodulo  , imodulo  , wmodulo  ,
};
