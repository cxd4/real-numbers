#include <float.h>
#include <signal.h>

#include "strtor.h"
#include <stdio.h>

#include "calc.h"
#include <math.h>

int wadd(int argc, char* argv[])
{
    whole_number answer, source;
    register int i;

    answer = 0;
    if (argc < 3)
        return -1;
    for (i = 1; i < argc; i++) {
        source = strtow(argv[i]);
        answer += source;
    }
    printf("%lu\n", answer);
    return 0;
}
int wsubtract(int argc, char* argv[])
{
    whole_number answer, source;
    register int i;

    if (argc < 3)
        return -1;
    answer = strtow(argv[1]);
    for (i = 2; i < argc; i++) {
        source = strtow(argv[i]);
        answer -= source;
    }
    printf("%lu\n", answer);
    return 0;
}
int wmultiply(int argc, char* argv[])
{
    whole_number answer, source;
    register int i;

    answer = 1;
    if (argc < 3)
        return -1;
    for (i = 1; i < argc; i++) {
        source = strtow(argv[i]);
        answer *= source;
    }
    printf("%lu\n", answer);
    return 0;
}
int wdivide(int argc, char* argv[])
{
    whole_number answer, source;
    register int i;

    if (argc < 3)
        return -1;
    answer = strtow(argv[1]);
    for (i = 2; i < argc; i++) {
        source = strtow(argv[i]);
#ifndef SIGFPE
        if (source == 0) {
            answer = ULONG_MAX;
            continue;
        }
#endif
        answer /= source;
    }
    printf("%lu\n", answer);
    return 0;
}
