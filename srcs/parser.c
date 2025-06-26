#include "commons.h"

/*
 * Print usage information for the program
*/
void print_help(void)
{
	printf("Usage: ft_ping [OPTION...] HOST ...\n");
	printf("Send ICMP ECHO_REQUEST packets to network hosts.\n");
	printf("\n");
	printf(" Options valid for all request types:\n");
	printf("\n");
	printf("  -c, --count=NUMBER         stop after sending NUMBER packets\n");
	printf("  -q, --quiet                quiet output\n");
	printf("  -s, --size=NUMBER          send NUMBER data octets\n");
	printf("      --ttl=N                specify N as time-to-live\n");
	printf("  -v, --verbose              verbose output\n");
	printf("  -w, --timeout=N            stop after N seconds\n");
	printf("  -?, --help                 give this help list\n");
	printf("      --usage                give a short usage message\n");
	printf("\n");
	printf("Mandatory or optional arguments to long options are also mandatory or optional\n");
	printf("for any corresponding short options.\n");
	printf("\n");
	printf("Report bugs to <nlederge@student.42mulhouse.fr>.\n");
}

void print_usage(void)
{
	printf("Usage: ft_ping [OPTION...] HOST ...\n");
	printf("Send ICMP ECHO_REQUEST packets to network hosts.\n");
}

/* Parses command line arguments and sets up program options
 * Sets default TTL to 64 if not specified
 * Supports --ttl=value flag for setting custom TTL value
 * Supports -c/--count=NUMBER to specify how many packets to send
 * Supports -s/--size=NUMBER to specify packet size
 * Supports -v/--verbose for verbose output
 * Supports -q/--quiet for quiet output (suppress normal output)
 * Supports -?/--help for help information
 * IP address can be specified at any position
*/
int parse_arguments(int argc, char *argv[], t_ping_context *context)
{
	context->ttl = 64;		
	context->destination_ip = NULL;
	context->count = -1;
	context->packet_size = 56;
	context->timeout = (time_t)1;

	for (int i = 1; i < argc; i++)
	{
		if (strncmp(argv[i], "--ttl=", 6) == 0)
		{
			context->ttl = atoi(argv[i] + 6);
			if (context->ttl <= 0 || context->ttl > 255)
			{
				fprintf(stderr, "Invalid TTL value. Must be between 1 and 255.\n");
				return (1);
			}
		}
		else if (strcmp(argv[i], "-c") == 0 || strncmp(argv[i], "--count=", 8) == 0)
		{
			if (strncmp(argv[i], "--count=", 8) == 0)
			{
				context->count = atoi(argv[i] + 8);
			}
			else
			{
				if (i + 1 >= argc)
				{
					fprintf(stderr, "ft_ping: option requires an argument -- 'c'\n");
					return (1);
				}
				context->count = atoi(argv[i + 1]);
				i++;
			}
			if (context->count <= 0)
			{
				context->count = -1;
				return (1);
			}
		}
		else if (strcmp(argv[i], "-s") == 0 || strncmp(argv[i], "--size=", 7) == 0)
		{
			if (strncmp(argv[i], "--size=", 7) == 0)
			{
				context->packet_size = atoi(argv[i] + 7);
			}
			else
			{
				if (i + 1 >= argc)
				{
					fprintf(stderr, "ft_ping: option requires an argument -- 's'\n");
					return (1);
				}
				context->packet_size = atoi(argv[i + 1]);
				i++;
			}
			if (context->packet_size <= 0 || context->packet_size > PING_MAX_DATALEN - sizeof(struct icmp))
			{
				fprintf(stderr, "Invalid packet size. Must be between 1 and %zu.\n", 
					PING_MAX_DATALEN - sizeof(struct icmp));
				return (1);
			}
		}
		else if (strcmp(argv[i], "-w") == 0 || strncmp(argv[i], "--timeout=", 10) == 0)
		{
			if (strncmp(argv[i], "--timeout=", 10) == 0)
			{
				context->timeout = (time_t)atoi(argv[i] + 10);
			}
			else
			{
				if (i + 1 >= argc)
				{
					fprintf(stderr, "ft_ping: option requires an argument -- 'w'\n");
					return (1);
				}
				context->timeout = (time_t)atoi(argv[i + 1]);
				i++;
			}
			if (context->timeout <= 0)
			{
				fprintf(stderr, "Invalid timeout value. Must be greater than 0.\n");
				return (1);
			}
		}
		else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0)
		{
			context->flags |= FLAG_VERBOSE;
		}
		else if (strcmp(argv[i], "-q") == 0 || strcmp(argv[i], "--quiet") == 0)
		{
			context->flags |= FLAG_QUIET;
		}
		else if (strcmp(argv[i], "-?") == 0 || strcmp(argv[i], "--help") == 0)
		{
			context->flags |= FLAG_HELP;
			print_help();
			return (2);
		}
		else if (strcmp(argv[i], "--usage") == 0)
		{
			print_usage();
			return (2);
		}
		else
		{
			context->destination_ip = argv[i];
		}
	}
	
	if (context->destination_ip == NULL)
	{
		fprintf(stderr, "ft_ping: missing host operand\n");
		fprintf(stderr, "Try 'ft_ping --help' or 'ft_ping --usage' for more information.\n");
		return (1);
	}
	return (0);
} 