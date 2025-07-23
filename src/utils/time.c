#include "commons.h"

/*
 * Gets the current timestamp using gettimeofday
*/
void	get_timestamp(struct timeval *timestamp)
{
	gettimeofday(timestamp, NULL);
}

/*
 * Calculates elapsed time in milliseconds between two timestamps
*/
double	get_elapsedtime_ms(struct timeval *start, struct timeval *end)
{
	return ((end->tv_sec - start->tv_sec) * 1000 + (end->tv_usec - start->tv_usec) / 1000.0);
}
