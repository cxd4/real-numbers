#include <float.h>
#include <signal.h>

#include "calc.h"
#include <math.h>

#include "strtor.h"
integer i_result;

int iadd(int argc, char* argv[])
{
    integer answer, source;
    register int i;

    answer = 0;
    if (argc < 3)
        return -1;
    for (i = 1; i < argc; i++) {
        source = strtoi(argv[i]);
        answer += source;
    }
    i_result = answer;
    return 0;
}
int isubtract(int argc, char* argv[])
{
    integer answer, source;
    register int i;

    if (argc < 3)
        return -1;
    answer = strtoi(argv[1]);
    for (i = 2; i < argc; i++) {
        source = strtoi(argv[i]);
        answer -= source;
    }
    i_result = answer;
    return 0;
}
int imultiply(int argc, char* argv[])
{
    integer answer, source;
    register int i;

    answer = 1;
    if (argc < 3)
        return -1;
    for (i = 1; i < argc; i++) {
        source = strtoi(argv[i]);
        answer *= source;
    }
    i_result = answer;
    return 0;
}
int idivide(int argc, char* argv[])
{
    integer answer, source;
    register int i;

    if (argc < 3)
        return -1;
    answer = strtoi(argv[1]);
    for (i = 2; i < argc; i++) {
        source = strtoi(argv[i]);
#ifndef SIGFPE
        if (source == 0) {
            answer = (answer < 0) ? LONG_MIN : LONG_MAX;
            continue;
        } /* Geometric graphs of (n / x) draw best from this approximation. */
#endif
        answer /= source;
    }
    i_result = answer;
    return 0;
}
int imodulo(int argc, char* argv[])
{
    integer answer, source;
    register int i;

    if (argc < 3)
        return -1;
    answer = strtoi(argv[1]);
    for (i = 2; i < argc; i++) {
        source = strtoi(argv[i]);
#ifndef SIGFPE
        if (source == 0)
            return 1;
#endif
        answer %= source;
    }
    i_result = answer;
    return 0;
}
