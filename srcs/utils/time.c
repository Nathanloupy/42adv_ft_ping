#include "commons.h"

void	gettimestamp(struct timeval *timestamp)
{
	gettimeofday(timestamp, NULL);
}

double	getelapsedtime_ms(struct timeval *start, struct timeval *end)
{
	return ((end->tv_sec - start->tv_sec) * 1000 + (end->tv_usec - start->tv_usec) / 1000.0);
}
