#ifndef _CALC_H_
#define _CALC_H_

typedef int(*math_operation)(
    int argc,
    char* argv[]
);

extern const math_operation op_functions[];

/*
 * operations on any real numbers that fit the CPU's floating-point precision
 */
typedef double          real;
extern int radd     (int argc, char* argv[]);
extern int rsubtract(int argc, char* argv[]);
extern int rmultiply(int argc, char* argv[]);
extern int rdivide  (int argc, char* argv[]);

/*
 * high-precision integer operations
 */
typedef long            integer;
extern int iadd     (int argc, char* argv[]);
extern int isubtract(int argc, char* argv[]);
extern int imultiply(int argc, char* argv[]);
extern int idivide  (int argc, char* argv[]);

/*
 * higher-precision whole number operations
 */
typedef unsigned long   whole_number;
extern int wadd     (int argc, char* argv[]);
extern int wsubtract(int argc, char* argv[]);
extern int wmultiply(int argc, char* argv[]);
extern int wdivide  (int argc, char* argv[]);

#endif
