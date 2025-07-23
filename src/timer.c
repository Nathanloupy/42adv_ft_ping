#include "commons.h"

/*
 * Waits for the next interval to maintain consistent packet timing
 * Calculates time to wait between send time and current time
 * Normalizes time values if microseconds are negative
 * Converts microseconds to nanoseconds for nanosleep
 * Only waits if there's positive time remaining
*/
void	wait_for_next_interval(struct timeval *last_send_time)
{
	struct	timeval current_time;
	struct	timespec req;
	
	get_timestamp(&current_time);
	
	long	wait_sec = last_send_time->tv_sec + 1 - current_time.tv_sec;
	long	wait_usec = last_send_time->tv_usec - current_time.tv_usec;
	
	if (wait_usec < 0)
	{
		wait_sec--;
		wait_usec += 1000000;
	}
	
	if (wait_sec >= 0 && (wait_sec > 0 || wait_usec > 0))
	{
		req.tv_sec = wait_sec;
		req.tv_nsec = wait_usec * 1000;
		
		nanosleep(&req, NULL);
	}
}
