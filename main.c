#include <stdio.h>
#include <float.h>

#include "queue.h"
#include "strtor.h"

#include <signal.h>
#include <setjmp.h>

static jmp_buf CPU_state;
static void FPU_exception_handler(int signal_code)
{
    fprintf(stderr, "Caught ALU exception with signal %i.\n", signal_code);
    longjmp(CPU_state, signal_code);
} /* overflow, division by zero, etc. */
static void user_interrupt_handler(int signal_code)
{
    fprintf(stdout, "\nUser interrupt request with signal %i.\n", signal_code);
    longjmp(CPU_state, signal_code);
}
static void seg_av_handler(int signal_code)
{
    fprintf(stderr, "Caught access violation with signal %i.\n", signal_code);
    longjmp(CPU_state, signal_code);
}

int
#ifdef SUPPORT_COMMAND_LINE_FROM_OS
main(int argc, char* argv[])
#else
main(void)
#endif
{
    int status_code;
    int recovered_from_exception;

    signal(SIGFPE, FPU_exception_handler);
    signal(SIGINT, user_interrupt_handler);
    signal(SIGSEGV, seg_av_handler); /* This one should never be needed.... */
    recovered_from_exception = setjmp(CPU_state);

    switch (recovered_from_exception) {
    case 0:
        break;
    case SIGFPE: /* probably divided by 0 on purpose or did, e.g., sqrt(-1) */
        signal(SIGFPE, FPU_exception_handler); /* Reschedule the EH callback. */
#ifdef SUPPORT_COMMAND_LINE_FROM_OS
        if (argc >= 2)
            return 0;
#endif
        fprintf(stdout, "%Le\n", LDBL_MAX/LDBL_MIN);
        break; /* Dive back in to bug the user for more crash attempts. */
    case SIGINT:
        return 0;
    default:
        fprintf(stderr,
            "Leaked over from unhandled signal %i!\n", recovered_from_exception
        );
        return (recovered_from_exception);
    }

#ifdef SUPPORT_COMMAND_LINE_FROM_OS
    if (argc < 2)
        return op_queue();
    status_code = f_execute(argc - 1, &argv[1]);

    switch (status_code) {
    case 0x7FFF: /* sent from "exit" commands */
    case 0:
        return 0;

    case -2:
        fprintf(stderr, "Unknown operation:  %s\n", argv[1]);
        return -2;
    case -1:
        puterr("Insufficient operands.");
        return -1;
    case +1:
        puterr("Requested operation cannot be executed.");
        return 1;
    case +2:
        puterr("Requested null or undefined operation.");
        return 2;
    default:
        fprintf(stderr, "Unknown operation error:  %i.\n", status_code);
    }
    return (status_code);
#else
    return op_queue();
#endif
}

/*
 * provided to reduce EXE file size of the MS-DOS build by DJGPP
 *
 * DJGPP      :  gcc -o rc_debug.exe *.c -Os -Wall -pedantic -lc -lm
 * Strip with :  strip -o rc.exe rc_debug.exe -s
 *
 * Open Watcom:  wcl386 -fe=rc *.c -d0 -os -s -bcl=stub32xc
 */
#ifdef __DJGPP__
#include <crt0.h>

char**
__crt0_glob_function(char* arg)
{
    return NULL;
}
void
__crt0_load_environment_file(char* progname)
{
    return;
}
#ifndef SUPPORT_COMMAND_LINE_FROM_OS
void
__crt0_setup_arguments(void)
{
    return;
}
#endif
#endif
