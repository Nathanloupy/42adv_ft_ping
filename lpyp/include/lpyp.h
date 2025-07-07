#pragma once

#include "libft.h"
#include <stdio.h>

/* Option flags for t_lpyp_options */
#define LPYP_NO_ARG         0x00    /* Option takes no argument */
#define LPYP_REQUIRED_ARG   0x01    /* Option requires an argument */
#define LPYP_OPTIONAL_ARG   0x02    /* Option has optional argument */
#define LPYP_DENY_DUPLICATE 0x04    /* Option cannot appear multiple times */

/* Special keys for non-option arguments */
#define LPYP_KEY_ARG        0x80000000  /* Non-option argument */
#define LPYP_KEY_END        0x80000001  /* End of arguments */
#define LPYP_KEY_UNKNOWN    0x80000002  /* Unknown option */

typedef struct s_lpyp_option
{
    char            short_name;     /* Short option name (e.g., 'v' for -v) */
    char            *long_name;     /* Long option name (e.g., "verbose" for --verbose) */
    unsigned int    key;            /* Unique identifier for this option */
    int             flags;          /* Combination of LPYP_* flags */
    char            *description;   /* Description for help text */
    char            *arg_name;      /* Name of argument in help (e.g., "FILE", "NUM") */
}   t_lpyp_option;

/* User callback function type */
typedef int (*t_lpyp_parser)(unsigned int key, void *data, char *argument);

/* Function prototypes */
int     lpyp_parse(void *data, int argc, char **argv, t_lpyp_option *options, t_lpyp_parser parser_func);
int     lpyp_help(t_lpyp_option *options, char *program_name, char *description);
int     lpyp_usage(t_lpyp_option *options, char *program_name);

 