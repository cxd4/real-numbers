#include <float.h>
#include <signal.h>

#include <stdlib.h>
#include <math.h>

#include <limits.h>
#include "calc.h"
#include "strtor.h"
whole_number w_result;

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
    w_result = answer;
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
    w_result = answer;
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
    w_result = answer;
    return 0;
}
int wdivide(int argc, char* argv[])
{
    whole_number answer, source, old_answer;
    register int i;

    if (argc < 3)
        return -1;
    answer = strtow(argv[1]);
    for (i = 2; i < argc; i++) {
        source = strtow(argv[i]);
        if (source == 0)
            return 1;
        old_answer = answer;
        answer /= source;
        if (old_answer != answer * source)
            return 1; /* loss of precision from integer divides */
    }
    w_result = answer;
    return 0;
}
int wmodulo(int argc, char* argv[])
{
    whole_number answer, source;
    register int i;

    if (argc < 3)
        return -1;
    answer = strtow(argv[1]);
    for (i = 2; i < argc; i++) {
        source = strtow(argv[i]);
        if (source == 0)
            return 1;
        answer %= source;
    }
    w_result = answer;
    return 0;
}
int wpower(int argc, char* argv[])
{
    whole_number answer;
    whole_number base, power;
    register int i;

    if (argc < 2)
        return -1;
    base = strtow(argv[1]);
    power = 1;
    for (i = 2; i < argc; i++)
        power *= strtow(argv[i]);

    answer = +1;
    while (power-- != 0)
        answer *= base;
    w_result = answer;
    return 0;
}
int wbexp(int argc, char* argv[])
{
    whole_number significand;
    integer source;
    unsigned int shift_amount;

    if (argc < 3)
        return -1;
    significand = strtow(argv[1]);
    source      = strtoi(argv[2]);
#if (LONG_MAX > INT_MAX || LONG_MIN < INT_MIN)
    if (source < INT_MIN || source > INT_MAX)
        return 1; /* exceeded domain boundaries if sizeof(int) < sizeof(long) */
#endif
    shift_amount = (unsigned int)(source < 0 ? -source : +source);
    w_result = significand;
    if (source < 0)
        w_result >>= shift_amount;
    else
        w_result <<= shift_amount;
    return 0;
}
int wbradix(int argc, char* argv[])
{
    real significand, source;
    int power;

    if (argc < 1)
        return -1;
    w_result = FLT_RADIX;
    if (argc < 3)
        return 0; /* Specify an optional argv[1] to test frexp() reciprocity. */
    source = strtor(argv[1]);
    significand = frexp(source, &power);
    if (pow(FLT_RADIX, power) * significand != source)
        return 1; /* Test failed...but why? */
    return 0;
}
int wfact(int argc, char* argv[])
{
    whole_number answer, old_answer;
    register whole_number i, j, n;

    if (argc < 2)
        return -1;
    n = strtow(argv[1]);

    answer = 1;
    for (i = 1; i <= n; i++) {
        old_answer = answer;
        for (j = 1; j < i; j++) {
            answer += old_answer;
            if (answer < old_answer)
                return 1; /* unsigned overflow */
        }
    }
    w_result = answer;
    return 0;
}
int wcomp(int argc, char* argv[])
{
    whole_number n0, n1;

    if (argc < 3)
        return -1;
    n0 = strtow(argv[1]);
    n1 = strtow(argv[2]);
    w_result = (n0 < n1) ? 1 : 0;
    return 0;
}
int wgcd(int argc, char* argv[])
{
    whole_number m, n;
    whole_number i, j; /* Loop from i = 1 to j = max(m, n). */

    if (argc < 3)
        return -1;
    m = strtow(argv[1]);
    n = strtow(argv[2]);
    if (argc > 3)
        return 1; /* Find GCF of several numbers in one command:  unsupported */
    if (m == n) {
        w_result = m;
        return 0;
    }

    w_result = 1;
    j = (m < n) ? n : m;
    for (i = 1; i < j; i++)
        if (m % i == 0 && n % i == 0)
            w_result = i;
    return 0;
}
int wlcm(int argc, char* argv[])
{
    whole_number m, n;
    whole_number answer;

    if (argc < 3)
        return -1;
    m = strtow(argv[1]);
    n = strtow(argv[2]);
    if (argc > 3)
        return 1; /* Find LCM of several numbers in one command:  unsupported */
    if (m == 0 || n == 0)
        return 1; /* The LCM of 0 and anything is infinite. */

    answer = (m < n) ? n : m;
    while (answer % m != 0 || answer % n != 0) {
        ++(answer);
        if (answer == 0)
            return 1; /* unsigned overflow trying to count toward the LCM */
    }
    w_result = answer;
    return 0;
}
int wmean(int argc, char* argv[])
{
    whole_number answer;
    register int i;

    answer = 0;
    if (argc < 2)
        return -1;
    for (i = 1; i < argc; i++)
        answer += strtow(argv[i]);
    w_result = answer / (argc - 1);
    return 0;
}
int wmedian(int argc, char* argv[])
{
    whole_number* constants;
    register size_t i, limit;

    if (argc < 2)
        return -1;
    limit = (unsigned int)(argc) - 1; /* argc isn't negative, so this fits. */
    constants = (whole_number*)realloc(NULL, sizeof(whole_number) * limit);
    if (constants == NULL)
        return 1;

    for (i = 0; i < limit; i++)
        constants[i] = strtow(argv[i + 1]);
    qsort(constants, limit, sizeof(whole_number), (qsort_cmp_func)wqsort_cmp);

    if (limit % 2) { /* An odd number of constants has only one middle const. */
        i = (limit - 1) >> 1;
        w_result = constants[i];
    } else { /* An even-numbered list of constants has two middles. */
        i = (limit >> 1) - 1;
        w_result = (constants[i + 0] + constants[i + 1]) / 2;
    }
    free(constants);
    return 0;
}
int wmode(int argc, char* argv[])
{
    whole_number_list* unique_numbers;
    whole_number* constants;
    whole_number most_repeats;
    register size_t i, j, limit, number_of_unique_entries;

    if (argc < 2)
        return -1;
    limit = (unsigned int)(argc) - 1;
    constants = (whole_number*)realloc(NULL, sizeof(whole_number) * limit);
    if (constants == NULL)
        return 1;

    for (i = 0; i < limit; i++)
        constants[i] = strtow(argv[i + 1]);
    qsort(constants, limit, sizeof(whole_number), (qsort_cmp_func)wqsort_cmp);

    number_of_unique_entries = 1;
    for (i = 1; i < limit; i++)
        if (constants[i] != constants[i - 1])
            ++(number_of_unique_entries);
    unique_numbers = (whole_number_list*)
        realloc(NULL, number_of_unique_entries * sizeof(whole_number_list));
    if (unique_numbers == NULL) {
        free(constants);
        return 1;
    }
    for (j = 0; j < number_of_unique_entries; j++)
        unique_numbers[j].repeats = 0;

    unique_numbers[0].constant = constants[0];
    for (j = 0, i = 1; i < limit; i++)
        if (constants[i] != constants[i - 1])
            unique_numbers[++j].constant = constants[i];
        else
            ++(unique_numbers[j].repeats);
    free(constants);

    w_result     = unique_numbers[0].constant;
    most_repeats = unique_numbers[0].repeats;
    for (j = 1; j < number_of_unique_entries; j++)
        if (most_repeats < unique_numbers[j].repeats) {
            most_repeats = unique_numbers[j].repeats;
            w_result = unique_numbers[j].constant;
        }
    free(unique_numbers);
    if (most_repeats == 0)
        return 1; /* This data set has no mode.  All constants appear once. */
    return 0;
}
int wrange(int argc, char* argv[])
{
    whole_number greatest, least, source;
    register int i;

    if (argc < 2)
        return -1;
    greatest = least = 0;
    for (i = 1; i < argc; i++) {
        source = strtow(argv[i]);
        if (least > source || i == 1)
            least = source;
        if (greatest < source || i == 1)
            greatest = source;
    }
    w_result = greatest - least;
    return 0;
}
int wasin(int argc, char* argv[])
{
    integer old_result;
    int error_status;

    old_result = i_result;
    error_status = iasin(argc, argv);
    w_result = (whole_number)(i_result + 360) % 360; /* -90 deg. = 270 deg. */
    i_result = old_result;
    return (error_status);
}
int wacos(int argc, char* argv[])
{
    integer old_result;
    int error_status;

    old_result = i_result;
    error_status = iacos(argc, argv);
    w_result = (whole_number)(i_result + 360) % 360;
    i_result = old_result;
    return (error_status);
}
int watan(int argc, char* argv[])
{
    integer old_result;
    int error_status;

    old_result = i_result;
    error_status = iatan(argc, argv);
    w_result = (whole_number)(i_result + 180) % 180;
    i_result = old_result;
    return (error_status);
}

int wexit(int argc, char* argv[])
{
    int signal_code;

    w_result = 0;
    if (argc >= 2)
        w_result = strtow(argv[1]);

    if (w_result > INT_MAX)
        w_result = SIGFPE; /* integer overflow exception */
    signal_code = (int)(w_result);
    if (signal_code != 0) {
        signal(signal_code, SIG_DFL);
        raise(signal_code);
    }
    return 0x7FFF;
}
