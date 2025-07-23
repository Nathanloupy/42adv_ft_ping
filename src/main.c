#include "commons.h"

/*
 * Main function for the ft_ping program
 * Initializes context, parses arguments, sets up socket, and handles the ping loop
*/
int	main(int argc, char *argv[])
{
	t_ping_context	context;
	int				result;
	struct timeval	last_send_time;
	int				seq_num = 0;
	time_t			start_time;
	time_t			current_time;

	memset(&context, 0, sizeof(context));
	context.stats.min_time = 9999.0;
	context.pid = getpid() & 0xFFFF;

	if (parse_arguments(argc, argv, &context))
		return (1);

	if (initialize_icmp_socket(&context, context.dest_host))
		return (1);

	signal(SIGINT, signal_handler);

	if (!inet_ntop(AF_INET, &(context.dest_addr.sin_addr), context.dest_ip, INET_ADDRSTRLEN))
	{
		perror("inet_ntop");
		close(context.socket_fd);
		return (1);
	}
	if (!(context.flags & FLAG_VERBOSE))
		printf("PING %s (%s): %zu data bytes\n", context.dest_host, context.dest_ip, context.packet_size);
	else
		printf("PING %s (%s): %zu data bytes, id 0x%xd = %d\n", context.dest_host, context.dest_ip, context.packet_size, context.pid, context.pid);

	get_timestamp(&last_send_time);
	time(&start_time);

	while (keep_running && (context.count == -1 || seq_num < context.count))
	{
		time(&current_time);
		if (context.timeout != -1 && difftime(current_time, start_time) >= context.timeout)
			break;

		prepare_icmp_header(&context.icmp_hdr, context.packet, seq_num++, context.packet_size, &context);
		
		context.stats.packets_sent++;
		if (send_packet(context.socket_fd, &context.dest_addr, 
				context.packet, sizeof(context.icmp_hdr) + context.packet_size, &last_send_time) < 0)
		{
			perror("sendto");
			break;
		}

		result = wait_for_reply(context.socket_fd, TIMEOUT_MS, &last_send_time, &context);
		if (!keep_running)
			break;
		else if (result < 0)
			break;
		else
			wait_for_next_interval(&last_send_time);
	}
	print_statistics(&context);
	close(context.socket_fd);
	return (0);
}
