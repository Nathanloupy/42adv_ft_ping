#include "commons.h"

/*
 * Calculates the IP/ICMP checksum for packet validation
*/
unsigned short	checksum(void *buffer, int length)
{
	unsigned short	*buf = buffer;
	unsigned int	sum = 0;
	unsigned short	result;

	for (sum = 0; length > 1; length -= 2)
		sum += *buf++;
	if (length == 1)
		sum += *(unsigned char*)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return (result);
}
