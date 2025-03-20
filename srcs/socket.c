#include "commons.h"

/*
 * Initializes the ICMP socket, sets TTL, and configures the destination address
*/
int initialize_icmp_socket(t_ping_context *context, char *address_to)
{
	struct protoent *proto;
	struct addrinfo hints, *result;
	int ret;

	proto = getprotobyname("icmp");
	if (!proto)
	{
		perror("getprotobyname");
		return (1);
	}
	context->socket_fd = socket(PF_INET, SOCK_RAW, proto->p_proto);
	if (context->socket_fd == -1)
	{
		perror("socket");
		return (1);
	}
	
	if (setsockopt(context->socket_fd, IPPROTO_IP, IP_TTL, &context->ttl, sizeof(context->ttl)) < 0)
	{
		perror("setsockopt (IP_TTL)");
		close(context->socket_fd);
		return (1);
	}
	
	memset(&context->dest_addr, 0, sizeof(context->dest_addr));
	context->dest_addr.sin_family = PF_INET;
	if (inet_pton(PF_INET, address_to, &context->dest_addr.sin_addr) <= 0)
	{
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_RAW;
		
		ret = getaddrinfo(address_to, NULL, &hints, &result);
		if (ret != 0)
		{
			fprintf(stderr, "ft_ping: unknown host %s\n", address_to);
			close(context->socket_fd);
			return (1);
		}
		
		memcpy(&context->dest_addr, result->ai_addr, sizeof(struct sockaddr_in));
		freeaddrinfo(result);
	}
	return (0);
} 