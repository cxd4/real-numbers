#include <float.h>
#include <signal.h>

#include <stdlib.h>
#include <math.h>

#include <signal.h>
#include "calc.h"
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
int rfact(int argc, char* argv[])
{
    real answer;
    register real i, n;

    if (argc < 2)
        return -1;
    n = strtor(argv[1]);
    if (n < 0)
        return 1; /* factorial undefined for negative numbers */

    answer = 1;
    for (i = 1; i < n + 1; i += 1)
        answer *= i;
    r_result = answer;
    return 0;
}
int rcomp(int argc, char* argv[])
{
    real n0, n1;

    if (argc < 3)
        return -1;
    n0 = strtor(argv[1]);
    n1 = strtor(argv[2]);
    if (n0 == n1)
        r_result = 0;
    else if (n0 < n1)
        r_result = -1;
    else if (n0 > n1)
        r_result = +1;
    else /* unordered comparisons, such as between NaN and infinity */
        r_result = NAN;
    return 0;
}
int rmean(int argc, char* argv[])
{
    real answer;
    register int i;

    answer = 0;
    if (argc < 2)
        return -1;
    for (i = 1; i < argc; i++)
        answer += strtor(argv[i]);
    r_result = answer / (argc - 1);
    return 0;
}
int rmedian(int argc, char* argv[])
{
    real* constants;
    register size_t i, limit;

    if (argc < 2)
        return -1;
    limit = (unsigned int)(argc) - 1; /* argc isn't negative, so this fits. */
    constants = (real*)malloc(sizeof(real) * limit);
    if (constants == NULL)
        return 1;

    for (i = 0; i < limit; i++)
        constants[i] = strtor(argv[i + 1]);
    qsort(constants, limit, sizeof(real), (qsort_cmp_func)rqsort_cmp);

    if (limit % 2) { /* An odd number of constants has only one middle const. */
        i = (limit - 1) >> 1;
        r_result = constants[i];
    } else { /* An even-numbered list of constants has two middles. */
        i = (limit >> 1) - 1;
        r_result = (constants[i + 0] + constants[i + 1]) / 2;
    }
    free(constants);
    return 0;
}
int rmode(int argc, char* argv[])
{
    real_list* unique_numbers;
    real* constants;
    whole_number most_repeats;
    register size_t i, j, limit, number_of_unique_entries;

    if (argc < 2)
        return -1;
    limit = (unsigned int)(argc) - 1;
    constants = (real*)malloc(sizeof(real) * limit);
    if (constants == NULL)
        return 1;

    for (i = 0; i < limit; i++)
        constants[i] = strtor(argv[i + 1]);
    qsort(constants, limit, sizeof(real), (qsort_cmp_func)rqsort_cmp);

    number_of_unique_entries = 1;
    for (i = 1; i < limit; i++)
        if (constants[i] != constants[i - 1])
            ++(number_of_unique_entries);
    unique_numbers = (real_list*)
        calloc(number_of_unique_entries, sizeof(real_list));
    if (unique_numbers == NULL) {
        free(constants);
        return 1;
    }

    unique_numbers[0].constant = constants[0];
    for (j = 0, i = 1; i < limit; i++)
        if (constants[i] != constants[i - 1])
            unique_numbers[++j].constant = constants[i];
        else
            ++(unique_numbers[j].repeats);
    free(constants);

    r_result     = unique_numbers[0].constant;
    most_repeats = unique_numbers[0].repeats;
    for (j = 1; j < number_of_unique_entries; j++)
        if (most_repeats < unique_numbers[j].repeats) {
            most_repeats = unique_numbers[j].repeats;
            r_result = unique_numbers[j].constant;
        }
    free(unique_numbers);
    if (most_repeats == 0)
        return 1; /* This data set has no mode.  All constants appear once. */
    return 0;
}
int rrange(int argc, char* argv[])
{
    real greatest, least, source;
    register int i;

    if (argc < 2)
        return -1;
    greatest = least = 0;
    for (i = 1; i < argc; i++) {
        source = strtor(argv[i]);
        if (least > source || i == 1)
            least = source;
        if (greatest < source || i == 1)
            greatest = source;
    }
    r_result = greatest - least;
    return 0;
}
int rasin(int argc, char* argv[])
{
    real answer, source;

    if (argc < 2)
        return -1;
    source = strtor(argv[1]);
    if (source < -1 || source > +1) /* domain error:  errno = EDOM */
        return 1; /* Executing asin() on such a value has undefined output. */
    answer = asin(source);
    r_result = answer;
    return 0;
}
int racos(int argc, char* argv[])
{
    real answer, source;

    if (argc < 2)
        return -1;
    source = strtor(argv[1]);
    if (source < -1 || source > +1) 
        return 1;
    answer = acos(source);
    r_result = answer;
    return 0;
}
int ratan(int argc, char* argv[])
{
    real answer, slope, rise, run;

    if (argc < 2)
        return -1;
    rise = strtor(argv[1]);
    if (argc < 3) {
        run = 1;
        answer = atan(slope = rise / run);
    } else {
        run = strtor(argv[2]);
        if (rise == 0 && run == 0)
            return 1; /* A line with no rise or run is just a single point. */
        answer = atan2(rise, run);
    }
    r_result = answer;
    return 0;
}

int rexit(int argc, char* argv[])
{
    if (argc >= 2) {
        r_result = strtor(argv[1]);
        if (r_result != 0)
            raise(SIGINT);
    }
    return 0xDEAD;
}

const math_operation op_functions[] = {
    radd     , iadd     , wadd     ,
    rsubtract, isubtract, wsubtract,
    rmultiply, imultiply, wmultiply,
    rdivide  , idivide  , wdivide  ,

    rmean    , imean    , wmean    ,
    rmedian  , imedian  , wmedian  ,
    rmode    , imode    , wmode    ,
    rrange   , irange   , wrange   ,

    rmodulo  , imodulo  , wmodulo  ,
    rabsval  , iabsval  , wnull    ,
    rceiling , inull    , wnull    ,
    rfloor   , inull    , wnull    ,

    rpower   , ipower   , wpower   ,
    rbexp    , ibexp    , wbexp    ,
    rroot    , inull    , inull    ,
    rbradix  , ibradix  , wbradix  ,

    rfact    , inull    , wfact    ,
    rcomp    , icomp    , wcomp    ,
    rnull    , inull    , wgcd     ,
    rnull    , inull    , wlcm     ,

    rasin    , iasin    , wasin    ,
    racos    , iacos    , wacos    ,
    ratan    , iatan    , watan    ,

    rexit    , iexit    , wexit    ,
};

int rqsort_cmp(const real* m, const real* n)
{
    if (*m < *n)
        return (0 - 1); /* Resort the array with m occurring before n. */
    if (*m > *n)
        return (0 + 1); /* Resort the array with m occurring after n. */
    return 0;
}
int iqsort_cmp(const integer* m, const integer* n)
{
    if (*m < *n)
        return (0 - 1);
    if (*m > *n)
        return (0 + 1);
    return 0;
}
int wqsort_cmp(const whole_number* m, const whole_number* n)
{
    if (*m < *n)
        return (0 - 1);
    if (*m > *n)
        return (0 + 1);
    return 0;
}
