#ifndef _STRTOR_H_
#define _STRTOR_H_

#include <stddef.h>

/*
 * Convert C strings to real numbers of the form (a + b*i; b = 0).
 * (i) is the imaginary unit, nullified by the coefficient (b = 0).
 */
#if 0
#define strtor(s)       strtod((s), NULL)
#define strtoi(s)       strtol((s), NULL, 0)
#define strtow(s)       strtoul((s), NULL, 0)
#endif

#include "calc.h"

/*
 * Interpret strings as floating-point real decimal numbers.
 * e.g. "3" as 3, "03." as 3, "3.14" as 3.14
 */
extern real
strtor(const char* str);

/*
 * Interpret strings as integer real numbers (using the longest C precision).
 * e.g. "3" as 3, "0x03" as 3, "-0x3.14" as -3
 */
extern integer
strtoi(const char* str);

/*
 * Interpret strings as whole numbers, allowing extra precision due to the
 * invalidity of negative integers under the definition of a whole number.
 */
extern whole_number
strtow(const char* str);


/*
 * number of digits of precision for type (real) preserving round-trip
 * accuracy when converting between exact values and printed strings
 *
 * The easiest standard solution to this is DBL_DECIMAL_DIG in C 2011.
 * Failing that, DECIMAL_DIG in C99 equates to LDBL_DECIMAL_DIG.
 * Failing that, some people only care about the safe DBL_DIG from ANSI C89.
 * Failing that (and not caring about any of that), we'll try calculating it.
 */
extern int
fp_str_round_trip(void);


/*
 * Note that several libraries exist to handle numbers with much more
 * precision than what (long double) allows (or (uintmax_t) and (intmax_t)
 * from C99).  This is certainly accomplished by using memory instead of CPU
 * registers for direct computation.  In the interest of keeping the program
 * direct, small and optimized, we want to use a native C type that will
 * generally (but not always) maintain direct register operations.
 *
 * Some exceptions to this are platforms like MS-DOS, where sizeof(long) of
 * course is going to exceed sizeof(size_t).  In this case, (int) operations
 * are certainly preferred for efficiency, but we still want fair precision.
 *
 * There is no interest in writing "long long" anywhere, so go away!
 * (...Except maybe ifdef _WIN64, which is LLP64 in MS' infinite wisdom.)
 */

/*
 * Since we don't want in-line expansion of the pi() function calls (partly
 * to maintain small executable size), we are going to be calling some kind
 * of function anyway when converting between radians and degrees.
 */
extern real to_degrees(real radians);
extern real to_radians(real degrees);

/*
 * Use puts()-style text printing using this function, which actually uses
 * fprintf() like the rest of the program to avoid linking in the puts() or
 * fputs() functions to keep the program file size down for platforms where
 * the program is built by statically linking in the C library functions.
 */
extern int
puterr(const char* message);

#endif
