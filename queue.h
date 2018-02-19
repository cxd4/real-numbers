#ifndef _QUEUE_H_
#define _QUEUE_H_

/*
 * If this is defined, main(argc, argv) will be used instead of main(void)
 * forcing an infinite loop of getting characters from stdin sent by user.
 *
 * If this is not defined, the latter will be the only executable flow.
 */
#define SUPPORT_COMMAND_LINE_FROM_OS

/*
 * Execute a user-defined operation text message.
 * system("rc add 2 2") will call f_execute(3, { "add", "2", "2" }).
 */
extern int
f_execute(int argc, char* argv[]);

/*
 * Recursively scan for text input by the user on the command line.
 * system("rc") will loop forever with each successful f_execute().
 */
extern int
op_queue(void);

#endif
