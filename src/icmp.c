#include "commons.h"

/*
 * Prepares and initializes an ICMP echo request packet with variable data size
*/
void	prepare_icmp_header(struct icmphdr *icmp_hdr, char *packet, int seq_num, size_t packet_size, t_ping_context *context)
{
	size_t	total_size = sizeof(*icmp_hdr) + packet_size;
	
	memset(icmp_hdr, 0, sizeof(*icmp_hdr));
	icmp_hdr->type = ICMP_ECHO;
	icmp_hdr->code = 0;
	icmp_hdr->un.echo.sequence = seq_num;
	icmp_hdr->un.echo.id = context->pid;

	memset(packet, 0, PING_MAX_DATALEN);
	for (size_t i = sizeof(*icmp_hdr); i < total_size; i++) 
		packet[i] = i % ICMP_DATA_PATTERN_MODULO;
	
	memcpy(packet, icmp_hdr, sizeof(*icmp_hdr));
	icmp_hdr->checksum = 0;
	icmp_hdr->checksum = checksum(packet, total_size);
	memcpy(packet, icmp_hdr, sizeof(*icmp_hdr));
}
