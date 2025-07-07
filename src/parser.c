#include "commons.h"

/* lpyp option definitions */
static t_lpyp_option g_options[] = {
	{'c', "count", 'c', LPYP_REQUIRED_ARG | LPYP_DENY_DUPLICATE, "stop after sending NUMBER packets", "NUMBER"},
	{'q', "quiet", 'q', LPYP_NO_ARG, "quiet output", NULL},
	{'s', "size", 's', LPYP_REQUIRED_ARG | LPYP_DENY_DUPLICATE, "send NUMBER data octets", "NUMBER"},
	{0, "ttl", 't', LPYP_REQUIRED_ARG | LPYP_DENY_DUPLICATE, "specify N as time-to-live", "N"},
	{'v', "verbose", 'v', LPYP_NO_ARG, "verbose output", NULL},
	{'w', "timeout", 'w', LPYP_REQUIRED_ARG | LPYP_DENY_DUPLICATE, "stop after N seconds", "N"},
	{'?', "help", '?', LPYP_NO_ARG, "give this help list", NULL},
	{0, "usage", 'u', LPYP_NO_ARG, "give a short usage message", NULL},
	{0, NULL, 0, 0, NULL, NULL} /* Sentinel */
};

/*
 * lpyp parser callback function
 */
static int ft_ping_parser(unsigned int key, void *data, char *argument)
{
	t_ping_context *context = (t_ping_context *)data;

	if (key == 'c')
	{
		context->count = ft_atoi(argument);
		if (context->count <= 0)
		{
			fprintf(stderr, "ft_ping: invalid count value: %s\n", argument);
			return (1);
		}
	}
	else if (key == 'q')
		context->flags |= FLAG_QUIET;
	else if (key == 's')
	{
		context->packet_size = ft_atoi(argument);
		if (context->packet_size <= 0 || context->packet_size > PING_MAX_DATALEN - sizeof(struct icmp))
		{
			fprintf(stderr, "ft_ping: invalid packet size: %s (must be between 1 and %zu)\n", 
				argument, PING_MAX_DATALEN - sizeof(struct icmp));
			return (1);
		}
	}
	else if (key == 't')
	{
		context->ttl = ft_atoi(argument);
		if (context->ttl <= 0 || context->ttl > 255)
		{
			fprintf(stderr, "ft_ping: invalid TTL value: %s (must be between 1 and 255)\n", argument);
			return (1);
		}
	}
	else if (key == 'v')
		context->flags |= FLAG_VERBOSE;
	else if (key == 'w')
	{
		context->timeout = (time_t)ft_atoi(argument);
		if (context->timeout <= 0)
		{
			fprintf(stderr, "ft_ping: invalid timeout value: %s\n", argument);
			return (1);
		}
	}
	else if (key == '?' || key == 'u')
	{
		if (key == '?')
		{
			context->flags |= FLAG_HELP;
			lpyp_help(g_options, "ft_ping", "Send ICMP ECHO_REQUEST packets to network hosts.");
		}
		else
			lpyp_usage(g_options, "ft_ping");
		return (2); /* Special return code for help/usage */
	}
	else if (key == LPYP_KEY_ARG)
	{
		if (context->destination_ip == NULL)
			context->destination_ip = argument;
		else
		{
			fprintf(stderr, "ft_ping: too many arguments\n");
			return (1);
		}
	}
	else if (key == LPYP_KEY_END)
	{
		if (context->destination_ip == NULL)
		{
			fprintf(stderr, "ft_ping: missing host operand\n");
			fprintf(stderr, "Try 'ft_ping --help' or 'ft_ping --usage' for more information.\n");
			return (1);
		}
	}
	else if (key == LPYP_KEY_UNKNOWN)
	{
		fprintf(stderr, "ft_ping: unrecognized option '%s'\n", argument);
		fprintf(stderr, "Try 'ft_ping --help' for more information.\n");
		return (1);
	}
	return (0);
}

/*
 * Initialize context with default values
 */
static void init_context_defaults(t_ping_context *context)
{
	context->ttl = 64;
	context->destination_ip = NULL;
	context->count = -1;
	context->packet_size = 56;
	context->timeout = (time_t)1;
	context->flags = 0;
}

/*
 * Parse command line arguments using lpyp
 */
int parse_arguments(int argc, char *argv[], t_ping_context *context)
{
	int ret;

	init_context_defaults(context);
	
	ret = lpyp_parse(context, argc, argv, g_options, ft_ping_parser);
	
	if (ret == 2)
		return (2); /* Help/usage was shown */
	else if (ret != 0)
		return (1); /* Error occurred */
	
	return (0); /* Success */
}

/*
 * Print usage information (kept for compatibility)
 */
void print_usage(void)
{
	lpyp_usage(g_options, "ft_ping");
} 