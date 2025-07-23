#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/select.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include "lpyp.h"

/* DEFINES - GENERAL */
#define MAXIPLEN 60
#define MAXICMPLEN 76
#define PING_MAX_DATALEN (65535 - MAXIPLEN - MAXICMPLEN)
#define TIMEOUT_MS 1000
#define DEFAULT_TTL 64
#define DEFAULT_PACKET_SIZE 56
#define ICMP_DATA_PATTERN_MODULO 256
#define DEFAULT_BUFFER_SIZE 2048

/* DEFINES - FLAGS */
#define FLAG_VERBOSE (1 << 0)  /* -v flag */
#define FLAG_HELP    (1 << 1)  /* -? flag */
#define FLAG_QUIET   (1 << 2)  /* -q flag */

/* DEFINES - ICMP ERRORS */
#ifndef ICMP_DEST_UNREACH
# define ICMP_DEST_UNREACH 3
#endif
#ifndef ICMP_SOURCE_QUENCH
# define ICMP_SOURCE_QUENCH 4
#endif
#ifndef ICMP_REDIRECT
# define ICMP_REDIRECT 5
#endif
#ifndef ICMP_TIME_EXCEEDED
# define ICMP_TIME_EXCEEDED 11
#endif
#ifndef ICMP_PARAMETERPROB
# define ICMP_PARAMETERPROB 12
#endif
#ifndef ICMP_NET_UNREACH
# define ICMP_NET_UNREACH 0
#endif
#ifndef ICMP_HOST_UNREACH
# define ICMP_HOST_UNREACH 1
#endif
#ifndef ICMP_PROT_UNREACH
# define ICMP_PROT_UNREACH 2
#endif
#ifndef ICMP_PORT_UNREACH
# define ICMP_PORT_UNREACH 3
#endif

/* UTILS */
#include "utils.h"

/* STRUCTS */
typedef struct s_stats {
		int packets_sent;
		int packets_received;
		double min_time;
		double max_time;
		double total_time;
		double sum_squared_time;
}	t_stats;

typedef struct s_ping_context
{
	pid_t				pid;
	int					socket_fd;
	struct sockaddr_in	dest_addr;
	int					ttl;
	char				*dest_host;
	char				dest_ip[INET_ADDRSTRLEN];
	int					count;
	struct icmphdr		icmp_hdr;
	char				packet[PING_MAX_DATALEN];
	size_t				packet_size;
	t_stats				stats;
	unsigned int		flags;
	time_t				timeout;
}	t_ping_context;

/* RETURN CODES */
enum e_ft_ping_ret_code
{
	FT_PING_SIGINT = -3,
	FT_PING_NO_REPLY = -2,
	FT_PING_ERROR = -1,
	FT_PING_OK = 0,
	FT_PING_TIMEOUT = 1,
};

/* GLOBAL VARIABLES */
extern volatile sig_atomic_t	keep_running;

/* SIGNALS */
void	signal_handler(int sig);

/* SOCKET */
int		initialize_icmp_socket(t_ping_context *context, char *dest_host);

/* ICMP */
void	prepare_icmp_header(struct icmphdr *icmp_hdr, char *packet, int seq_num, size_t packet_size, t_ping_context *context);

/* PACKET */
int		send_packet(int socket_fd, struct sockaddr_in *dest_addr, char *packet, 
                size_t packet_size, struct timeval *send_time);
int		process_received_packet(int socket_fd, struct timeval *start, t_ping_context *context);
int		wait_for_reply(int socket_fd, long timeout_ms, struct timeval *send_time, t_ping_context *context);

/* TIMER */
void	wait_for_next_interval(struct timeval *last_send_time);

/* PARSER */
int		parse_arguments(int argc, char *argv[], t_ping_context *context);
void	print_usage(void);

/* STATS */
void	update_stats(t_ping_context *context, double elapsed_time);
void	print_statistics(t_ping_context *context);
