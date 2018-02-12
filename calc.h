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
extern int rmodulo  (int argc, char* argv[]);
extern int rabsval  (int argc, char* argv[]);
extern int rceiling (int argc, char* argv[]);
extern int rfloor   (int argc, char* argv[]);
extern int rpower   (int argc, char* argv[]);
extern int rbexp    (int argc, char* argv[]);
extern int rroot    (int argc, char* argv[]);
extern int rbradix  (int argc, char* argv[]);
extern int rfact    (int argc, char* argv[]);
extern int rcomp    (int argc, char* argv[]);
/* not really useful:  rgcd() */
/* not really useful:  rlcm() */
extern int rmedian  (int argc, char* argv[]);

/*
 * high-precision integer operations
 */
typedef long            integer;
extern int iadd     (int argc, char* argv[]);
extern int isubtract(int argc, char* argv[]);
extern int imultiply(int argc, char* argv[]);
extern int idivide  (int argc, char* argv[]);
extern int imodulo  (int argc, char* argv[]);
extern int iabsval  (int argc, char* argv[]);
/* null operation:  iceiling() */
/* null operation:  ifloor() */
extern int ipower   (int argc, char* argv[]);
extern int ibexp    (int argc, char* argv[]);
/* not really useful:  iroot() */
extern int ibradix  (int argc, char* argv[]);
/* not really useful:  ifact() */
extern int icomp    (int argc, char* argv[]);
/* not really useful:  igcd() */
/* not really useful:  ilcm() */
extern int imedian  (int argc, char* argv[]);

/*
 * higher-precision whole number operations
 */
typedef unsigned long   whole_number;
extern int wadd     (int argc, char* argv[]);
extern int wsubtract(int argc, char* argv[]);
extern int wmultiply(int argc, char* argv[]);
extern int wdivide  (int argc, char* argv[]);
extern int wmodulo  (int argc, char* argv[]);
extern int wabsval  (int argc, char* argv[]);
/* null operation:  wceiling() */
/* null operation:  wfloor() */
extern int wpower   (int argc, char* argv[]);
extern int wbexp    (int argc, char* argv[]);
/* not really useful:  wroot() */
extern int wbradix  (int argc, char* argv[]);
extern int wfact    (int argc, char* argv[]);
extern int wcomp    (int argc, char* argv[]);
extern int wgcd     (int argc, char* argv[]);
extern int wlcm     (int argc, char* argv[]);
extern int wmedian  (int argc, char* argv[]);

/*
 * Store the last answer calculated for use in on-screen display later
 * as well as possible sourcing in future calculations.
 */
extern real             r_result;
extern integer          i_result;
extern whole_number     w_result;

/*
 * sort-by-comparision functions called by qsort() from stdlib.h
 */
typedef int(*qsort_cmp_func)(
    const void* m,
    const void* n
);
extern int rqsort_cmp(const real* m, const real* n);
extern int iqsort_cmp(const integer* m, const integer* n);
extern int wqsort_cmp(const whole_number* m, const whole_number* n);

#endif
