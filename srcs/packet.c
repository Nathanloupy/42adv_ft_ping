#include "commons.h"

/*
 * Sends an ICMP packet and records timestamp
*/
int send_packet(int socket_fd, struct sockaddr_in *dest_addr, char *packet, 
                size_t packet_size, struct timeval *send_time, t_ping_context *context)
{
	int ret;
	
	gettimestamp(send_time);
	// Stats are now updated in the calling function

	ret = sendto(socket_fd, packet, packet_size, 0, 
			(struct sockaddr*)dest_addr, sizeof(*dest_addr));
	
	if ((context->flags & FLAG_VERBOSE) && !(context->flags & FLAG_QUIET))
		printf("Sending %zu bytes ICMP packet to %s\n", 
			packet_size, context->destination_ip);
	
	if (ret <= 0)
		perror("sendto");
		
	return (ret);
}

/*
 * Waits for a response with timeout using select
*/
int wait_for_response(int socket_fd, fd_set *read_fds, t_ping_context *context)
{
	struct timeval timeout;
	
	(void)context;
	FD_ZERO(read_fds);
	FD_SET(socket_fd, read_fds);
	
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	
	int ret = select(socket_fd + 1, read_fds, NULL, NULL, &timeout); //TODO: test with 127.0.0.1, will not catch it, loop for select, display wrong packets, until timeout has passed
	
	if (ret < 0 && errno == EINTR)
		return (0);
	return (ret);
}

/*
 * Processes a received ICMP echo reply
*/
void process_echo_reply(char *recv_buf, struct sockaddr_in *from_addr, 
                        struct timeval *start, struct timeval *end, t_ping_context *context)
{
	char from_ip[INET_ADDRSTRLEN];
	double elapsed_time;
	struct ip* ip_hdr = (struct ip*)recv_buf;
	int ip_header_len = ip_hdr->ip_hl * 4;
	struct icmp* icmp_reply = (struct icmp*)(recv_buf + ip_header_len);

	inet_ntop(AF_INET, &(from_addr->sin_addr), from_ip, INET_ADDRSTRLEN);
	elapsed_time = getelapsedtime_ms(start, end);
	
	context->stats.packets_received++;
	context->stats.total_time += elapsed_time;
	context->stats.sum_squared_time += elapsed_time * elapsed_time;
	if (elapsed_time < context->stats.min_time)
		context->stats.min_time = elapsed_time;
	if (elapsed_time > context->stats.max_time)
		context->stats.max_time = elapsed_time;

	int icmp_size = context->packet_size + 8;
	
	if (!(context->flags & FLAG_QUIET))
	{
		printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n", 
			icmp_size, from_ip, icmp_reply->icmp_seq, 
			ip_hdr->ip_ttl, elapsed_time);
	}
}

/*
 * Handles different ICMP error message types
*/
void process_icmp_error(char *recv_buf, struct sockaddr_in *from_addr)
{
	char from_ip[INET_ADDRSTRLEN];
	struct ip* ip_hdr = (struct ip*)recv_buf;
	int ip_header_len = ip_hdr->ip_hl * 4;
	struct icmp* icmp_reply = (struct icmp*)(recv_buf + ip_header_len);
	
	inet_ntop(AF_INET, &(from_addr->sin_addr), from_ip, INET_ADDRSTRLEN);
	
	switch (icmp_reply->icmp_type)
	{
		case ICMP_DEST_UNREACH:
			printf("From %s: Destination Unreachable", from_ip);
			switch (icmp_reply->icmp_code)
			{
				case ICMP_NET_UNREACH:
					printf(" (Network Unreachable)\n");
					break;
				case ICMP_HOST_UNREACH:
					printf(" (Host Unreachable)\n");
					break;
				case ICMP_PROT_UNREACH:
					printf(" (Protocol Unreachable)\n");
					break;
				case ICMP_PORT_UNREACH:
					printf(" (Port Unreachable)\n");
					break;
				default:
					printf(" (code=%d)\n", icmp_reply->icmp_code);
					break;
			}
			break;
		case ICMP_TIME_EXCEEDED:
			printf("From %s: Time to Live exceeded\n", from_ip);
			break;
		case ICMP_PARAMETERPROB:
			printf("From %s: Parameter Problem\n", from_ip);
			break;
		case ICMP_SOURCE_QUENCH:
			printf("From %s: Source Quench\n", from_ip);
			break;
		case ICMP_REDIRECT:
			printf("From %s: Redirect\n", from_ip);
			break;
		case ICMP_ECHO:
			printf("From %s: Echo Request\n", from_ip);
			break;
		case ICMP_ECHOREPLY:
			printf("From %s: Echo Reply (not for us, ID mismatch)\n", from_ip);
			break;
		default:
			printf("From %s: Unknown ICMP type=%d, code=%d\n", 
				from_ip, icmp_reply->icmp_type, icmp_reply->icmp_code);
			break;
	}
}

/*
 * Processes a received packet and handles different ICMP message types
*/
int process_received_packet(int socket_fd, struct timeval *start, t_ping_context *context)
{
	char recv_buf[512];
	struct sockaddr_in from_addr;
	socklen_t from_len = sizeof(from_addr);
	struct timeval end;
	int ret;
	
	ret = recvfrom(socket_fd, recv_buf, sizeof(recv_buf), 0, 
			(struct sockaddr*)&from_addr, &from_len);
	
	if (ret <= 0) 
	{
		if (errno == EINTR && !keep_running)
			return (-1);
		perror("recvfrom");
		return (0);
	}
	
	gettimestamp(&end);
	struct ip* ip_hdr = (struct ip*)recv_buf;
	int ip_header_len = ip_hdr->ip_hl * 4;
	struct icmp* icmp_reply = (struct icmp*)(recv_buf + ip_header_len);

	char from_ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &from_addr.sin_addr, from_ip, INET_ADDRSTRLEN);

	if (context->flags & FLAG_VERBOSE)
		printf("%d bytes from %s: type = %d, code = %d\n", 
			ret, from_ip, icmp_reply->icmp_type, icmp_reply->icmp_code);

	if (icmp_reply->icmp_type == ICMP_ECHOREPLY && icmp_reply->icmp_id == (getpid() & 0xFFFF))
		process_echo_reply(recv_buf, &from_addr, start, &end, context);
	else if (!(context->flags & FLAG_QUIET))
		process_icmp_error(recv_buf, &from_addr);
		
	return (0);
} 