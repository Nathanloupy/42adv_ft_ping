#include "commons.h"

/*
 * Prepares and initializes an ICMP echo request packet with variable data size
*/
void prepare_icmp_header(struct icmp *icmp_hdr, char *packet, int sequence, size_t packet_size)
{
	size_t total_size = sizeof(*icmp_hdr) + packet_size;
	
	memset(icmp_hdr, 0, sizeof(*icmp_hdr));
	icmp_hdr->icmp_type = ICMP_ECHO;
	icmp_hdr->icmp_code = 0;
	icmp_hdr->icmp_seq = sequence;
	icmp_hdr->icmp_id = getpid() & 0xFFFF;

	// Fill the packet with pattern (0-255 repeated)
	memset(packet, 0, PING_MAX_DATALEN);
	for (size_t i = sizeof(*icmp_hdr); i < total_size; i++) 
		packet[i] = i % 256;
	
	memcpy(packet, icmp_hdr, sizeof(*icmp_hdr));
	icmp_hdr->icmp_cksum = 0;
	icmp_hdr->icmp_cksum = checksum(packet, total_size);
	memcpy(packet, icmp_hdr, sizeof(*icmp_hdr));
} 