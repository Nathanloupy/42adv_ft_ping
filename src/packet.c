#include "commons.h"

/*
 * Processes a received ICMP echo reply
*/
static void	process_echo_reply(const char *from_ip, int icmp_size, struct icmphdr *icmp_packet, struct iphdr *ip_hdr,
                        struct timeval *start, struct timeval *end, t_ping_context *context)
{
	double	elapsed_time;

	elapsed_time = get_elapsedtime_ms(start, end);
	
	update_stats(context, elapsed_time);
	
	if (!(context->flags & FLAG_QUIET))
	{
		printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n", 
			icmp_size, from_ip, icmp_packet->un.echo.sequence, 
			ip_hdr->ttl, elapsed_time);
	}
}

/*
 * Handles different ICMP error message types
*/
static void	process_other_icmp(const char *from_ip, int icmp_size, struct icmphdr *icmp_packet)
{
	switch (icmp_packet->type)
	{
		case ICMP_ECHOREPLY:
			printf("%d bytes from %s: Echo Reply (PID Mismatch)\n", icmp_size, from_ip);
			break;
		case ICMP_DEST_UNREACH:
			printf("%d bytes from %s: Destination Unreachable\n", icmp_size, from_ip);
			break;
		case ICMP_SOURCE_QUENCH:
			printf("%d bytes from %s: Source Quench\n", icmp_size, from_ip);
			break;
		case ICMP_REDIRECT:
			printf("%d bytes from %s: Redirect\n", icmp_size, from_ip);
			break;
		case ICMP_ECHO:
			printf("%d bytes from %s: Echo Request\n", icmp_size, from_ip);
			break;
		case ICMP_TIME_EXCEEDED:
			printf("%d bytes from %s: Time to Live exceeded\n", icmp_size, from_ip);
			break;
		case ICMP_PARAMETERPROB:
			printf("%d bytes from %s: Parameter Problem\n", icmp_size, from_ip);
			break;
		case ICMP_TIMESTAMP:
			printf("%d bytes from %s: Timestamp Request\n", icmp_size, from_ip);
			break;
		case ICMP_TIMESTAMPREPLY:
			printf("%d bytes from %s: Timestamp Reply\n", icmp_size, from_ip);
			break;
		case ICMP_INFO_REQUEST:
			printf("%d bytes from %s: Information Request\n", icmp_size, from_ip);
			break;
		case ICMP_INFO_REPLY:
			printf("%d bytes from %s: Information Reply\n", icmp_size, from_ip);
			break;
		case ICMP_ADDRESS:
			printf("%d bytes from %s: Address Mask Request\n", icmp_size, from_ip);
			break;
		case ICMP_ADDRESSREPLY:
			printf("%d bytes from %s: Address Mask Reply\n", icmp_size, from_ip);
			break;
		default:
			printf("%d bytes from %s: Unknown ICMP type=%d, code=%d\n", 
				icmp_size, from_ip, icmp_packet->type, icmp_packet->code);
			break;
	}
}

/*
 * Processes a received packet and handles different ICMP message types
*/
int	process_received_packet(int socket_fd, struct timeval *start, t_ping_context *context)
{
	char				recv_buf[DEFAULT_BUFFER_SIZE];
	struct sockaddr_in	from_addr;
	socklen_t			from_len = sizeof(from_addr);
	struct timeval		end;
	int 				ret;
	
	ret = recvfrom(socket_fd, recv_buf, sizeof(recv_buf), 0, 
			(struct sockaddr*)&from_addr, &from_len);

	if (ret < 0)
	{
		if (errno == EINTR && !keep_running)
			return (FT_PING_SIGINT);
		else if (errno == EINTR)
			return (FT_PING_NO_REPLY);
		perror("recvfrom");
		return (FT_PING_ERROR);
	}
	else if (!ret)
		return (FT_PING_NO_REPLY);

	get_timestamp(&end);

	char			from_ip[INET_ADDRSTRLEN];
	struct iphdr	*ip_hdr = (struct iphdr*)recv_buf;
	int				ip_header_len = ip_hdr->ihl * 4;
	struct icmphdr	*icmp_packet = (struct icmphdr*)(recv_buf + ip_header_len);
	int				icmp_size;

	if (!inet_ntop(AF_INET, &from_addr.sin_addr, from_ip, INET_ADDRSTRLEN))
	{
		perror("inet_ntop");
		return (FT_PING_ERROR);
	}
	icmp_size = ntohs(ip_hdr->tot_len) - sizeof(struct iphdr);
	if (icmp_packet->type == ICMP_ECHOREPLY && icmp_packet->un.echo.id == context->pid)
	{
		process_echo_reply(from_ip, icmp_size, icmp_packet, ip_hdr, start, &end, context);
		return (FT_PING_OK);
	}
	else if (!(context->flags & FLAG_QUIET))
	{
		process_other_icmp(from_ip, icmp_size, icmp_packet);
		return (FT_PING_NO_REPLY);
	}
	return (FT_PING_OK);
}

/*
 * Sends an ICMP packet and records timestamp
*/
int	send_packet(int socket_fd, struct sockaddr_in *dest_addr, char *packet, 
								size_t packet_size, struct timeval *send_time)
{
	get_timestamp(send_time);
	return (sendto(socket_fd, packet, packet_size, 0, 
				(struct sockaddr*)dest_addr, sizeof(*dest_addr)));
}
