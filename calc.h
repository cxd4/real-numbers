#ifndef _CALC_H_
#define _CALC_H_

typedef int(*math_operation)(
    int argc,
    char* argv[]
);

extern const math_operation op_functions[];

#endif
