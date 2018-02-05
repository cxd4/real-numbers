#include <float.h>
#include <signal.h>

#include "calc.h"
#include <math.h>

#include "strtor.h"
real r_result;

static int rnull(int argc, char* argv[])
{
    r_result = 0;
    if (argc >= 2)
        r_result = strtor(argv[1]);
    return 2;
}
static int inull(int argc, char* argv[])
{
    i_result = 0;
    if (argc >= 2)
        i_result = strtoi(argv[1]);
    return 2;
}
static int wnull(int argc, char* argv[])
{
    w_result = 0;
    if (argc >= 2)
        w_result = strtow(argv[1]);
    return 2;
}

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
    real answer, source;
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
int rabsval(int argc, char* argv[])
{
    real source;

    if (argc < 2)
        return -1;
    source = strtor(argv[1]);
    r_result = fabs(source);
    return 0;
}
int rceiling(int argc, char* argv[])
{
    real source;

    if (argc < 2)
        return -1;
    source = strtor(argv[1]);
    r_result = ceil(source);
    return 0;
}
int rfloor(int argc, char* argv[])
{
    real source;

    if (argc < 2)
        return -1;
    source = strtor(argv[1]);
    r_result = floor(source);
    return 0;
}
int rpower(int argc, char* argv[])
{
    real answer, source;
    register int i;

    if (argc < 3)
        return -1;
    answer = strtor(argv[1]);
    for (i = 2; i < argc; i++) {
        source = strtor(argv[i]);
        answer = pow(answer, source);
    }
    r_result = answer;
    return 0;
}
int rbexp(int argc, char* argv[])
{
    real significand;
    integer source;
    int power;

    if (argc < 3)
        return -1;
    significand = strtor(argv[1]);
    source      = strtoi(argv[2]);
#if (LONG_MAX > INT_MAX || LONG_MIN < INT_MIN)
    if (source < INT_MIN || source > INT_MAX)
        return 1; /* exceeded domain boundaries if sizeof(int) < sizeof(long) */
#endif
    power = (int)source;
    r_result = ldexp(significand, power);
    return 0;
}
int rroot(int argc, char* argv[])
{
    real answer, source;
    real index;
    int is_odd_root, negative_radicand_hack; /* Make root(-8, 3) == cbrt(-8). */
    register int i;

    if (argc < 2)
        return -1;
    answer = strtor(argv[1]);
    if (argc < 3) {
        r_result = sqrt(answer);
        return 0;
    }
    for (i = 2; i < argc; i++) {
        index = strtor(argv[i]);
        source = index;
        while (source != floor(source))
            source *= 10;
        is_odd_root = ((integer)(source) % 2 != 0) ? 1 : 0;
        negative_radicand_hack = is_odd_root && (answer < 0 ? 1 : 0);
        if (negative_radicand_hack)
            answer = -answer;
        answer = pow(answer, 1 / index); /* pow(-8, 1/3) is NaN; cbrt(-8) = 2 */
        if (negative_radicand_hack)
            answer = -answer;
    }
    r_result = answer;
    return 0;
}
int rbradix(int argc, char* argv[])
{
    real significand, source;
    int power;

    if (argc < 2)
        return -1;
    source = strtor(argv[1]);
    significand = frexp(source, &power);
    r_result = significand;
    return 0;
}

const math_operation op_functions[] = {
    radd     , iadd     , wadd     ,
    rsubtract, isubtract, wsubtract,
    rmultiply, imultiply, wmultiply,
    rdivide  , idivide  , wdivide  ,

    rmodulo  , imodulo  , wmodulo  ,
    rabsval  , iabsval  , wnull    ,
    rceiling , inull    , wnull    ,
    rfloor   , inull    , wnull    ,

    rpower   , ipower   , wpower   ,
    rbexp    , ibexp    , wbexp    ,
    rroot    , inull    , inull    ,
    rbradix  , ibradix  , wbradix  ,
};
