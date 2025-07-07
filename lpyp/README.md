# LPYP - Command Line Argument Parser for C

A command line argument parser for C, built using only libft functions.

## Quick Start

### 1. Define Your Options

```c
t_lpyp_option options[] = {
    {'v', "verbose", 'v', LPYP_NO_ARG, "Enable verbose output", NULL},
    {'o', "output", 'o', LPYP_REQUIRED_ARG | LPYP_DENY_DUPLICATE, "Specify output file", "FILE"},
    {'h', "help", 'h', LPYP_NO_ARG, "Show this help message", NULL},
    {0, NULL, 0, 0, NULL, NULL} /* Sentinel */
};
```

### 2. Create a Parser Function

```c
int my_parser(unsigned int key, void *data, char *argument)
{
    switch (key)
    {
        case 'v':
            ((my_data_t*)data)->verbose = 1;
            break;
        case 'o':
            ((my_data_t*)data)->output_file = argument;
            break;
        case 'h':
            /* Handle help request */
            break;
        case LPYP_KEY_ARG:
            /* Handle non-option arguments */
            break;
        case LPYP_KEY_END:
            /* Parsing finished */
            break;
        default:
            return (1); /* Error */
    }
    return (0);
}
```

### 3. Parse Arguments

```c
my_data_t data = {0};
if (lpyp_parse(&data, argc, argv, options, my_parser) != 0)
{
    lpyp_usage(options, argv[0]);
    return (1);
}
```

## API Reference

### Structures

#### `t_lpyp_option`

```c
typedef struct s_lpyp_option
{
    char            short_name;     /* Single character option (-v) */
    char            *long_name;     /* Long option name (--verbose) */
    unsigned int    key;            /* Unique identifier */
    int             flags;          /* Option behavior flags */
    char            *description;   /* Help text description */
    char            *arg_name;      /* Argument name for help (e.g., "FILE") */
}   t_lpyp_option;
```

### Flags

- `LPYP_NO_ARG`: Option takes no argument
- `LPYP_REQUIRED_ARG`: Option requires an argument
- `LPYP_OPTIONAL_ARG`: Option has an optional argument
- `LPYP_DENY_DUPLICATE`: Option cannot appear multiple times

### Special Keys

- `LPYP_KEY_ARG`: Non-option argument encountered
- `LPYP_KEY_END`: End of argument parsing
- `LPYP_KEY_UNKNOWN`: Unknown option encountered

### Functions

#### `lpyp_parse`

```c
int lpyp_parse(void *data, int argc, char **argv, t_lpyp_option *options, t_lpyp_parser parser_func);
```

Main parsing function. Returns 0 on success, 1 on error.

#### `lpyp_help`

```c
int lpyp_help(t_lpyp_option *options, char *program_name, char *description);
```

Prints detailed help information including usage and option descriptions.

#### `lpyp_usage`

```c
int lpyp_usage(t_lpyp_option *options, char *program_name);
```

Prints concise usage information.

## Examples

### Basic Usage

```bash
./program -v --output=results.txt input.txt
./program -vh -o results.txt input.txt
./program --verbose --help
```

### Duplicate Option Control

```bash
# Options without LPYP_DENY_DUPLICATE can be repeated
./program -v -v --verbose          # ✅ Works - multiple verbose flags

# Options with LPYP_DENY_DUPLICATE cannot be repeated  
./program --output=file1 --output=file2  # ❌ Error: option already specified
./program -d --debug                     # ❌ Error: option already specified
```

### Error Handling

```bash
./program -o              # Error: missing required argument
./program --unknown       # Error: unrecognized option
./program -x              # Error: invalid option
```

### Special Cases

```bash
./program file.txt -- --not-an-option    # Everything after -- treated as arguments
./program --help                         # Show help
./program                                # Show usage
```

## Building

```bash
make all     # Build library only
make clean   # Clean object files
make fclean  # Clean everything
```
