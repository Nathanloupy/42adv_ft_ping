#include "commons.h"

/*
 * Main function for the ft_ping program
 * Initializes context, parses arguments, sets up socket, and handles the ping loop
*/
int main(int argc, char *argv[])
{
	t_ping_context	context;
	int				ret;
	struct timeval	last_send_time;
	int				sequence = 0;
	fd_set			read_fds;
	time_t			start_time;
	time_t			current_time;

	memset(&context, 0, sizeof(context));
	context.stats.min_time = 9999.0;

	if (parse_arguments(argc, argv, &context))
		return (1);

	if (initialize_icmp_socket(&context, context.destination_ip))
		return (1);

	signal(SIGINT, signal_handler);

	char	from_ip[INET_ADDRSTRLEN];
	if (!inet_ntop(AF_INET, &(context.dest_addr.sin_addr), from_ip, INET_ADDRSTRLEN))
	{
		perror("inet_ntop");
		close(context.socket_fd);
		return (1);
	}
	printf("PING %s (%s): %zu data bytes\n", context.destination_ip, from_ip, context.packet_size);

	gettimestamp(&last_send_time);
	time(&start_time);

	while (keep_running && (context.count == -1 || sequence < context.count))
	{
		time(&current_time);
		if (difftime(current_time, start_time) >= context.timeout)
			break;

		prepare_icmp_header(&context.icmp_hdr, context.packet, sequence++, context.packet_size);
		
		context.stats.packets_sent++;
		if (send_packet(context.socket_fd, &context.dest_addr, 
				context.packet, sizeof(context.icmp_hdr) + context.packet_size, &last_send_time, &context) <= 0)
			break;

		ret = wait_for_response(context.socket_fd, &read_fds, &context);
		
		if (!keep_running)
			break;
		else if (ret < 0)
		{
			perror("select");
			break;
		}
		else if (ret == 0)
		{
			if (!(context.flags & FLAG_QUIET))
				printf("Request timeout for icmp_seq %d\n", context.icmp_hdr.icmp_seq);
		}
		else
		{
			if (process_received_packet(context.socket_fd, &last_send_time, &context) < 0)
				break;
			wait_for_next_interval(&last_send_time);
		}
	}
	print_statistics(&context);
	close(context.socket_fd);
	return (0);
}
