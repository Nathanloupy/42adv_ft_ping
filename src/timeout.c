#include "commons.h"

/*
 * Calculates the timeout for the select function
*/
static int	calculate_timeout(struct timeval *tv, long timeout_ms, struct timeval *send_time)
{
	struct timeval	now;
	get_timestamp(&now);

	long	elapsed = (long)get_elapsedtime_ms(send_time, &now);
	long	remain = timeout_ms - elapsed;

	if (remain <= 0)
		return (0);

	tv->tv_sec = remain / 1000;
	tv->tv_usec = (remain % 1000) * 1000;
	return (1);
}

/*
 * Waits for a response with timeout using select
*/
int	wait_for_reply(int socket_fd, long timeout_ms, struct timeval *send_time, t_ping_context *context)
{
	struct timeval	tv;
	int				ret;
	fd_set			rfds;

	while (keep_running)
	{
		if (calculate_timeout(&tv, timeout_ms, send_time) == 0)
			return (FT_PING_TIMEOUT);

		FD_ZERO(&rfds);
		FD_SET(socket_fd, &rfds);

		ret = select(socket_fd + 1, &rfds, NULL, NULL, &tv);
		if (ret < 0)
		{
			if (errno == EINTR)
				continue;
			perror("select");
			return (FT_PING_ERROR);
		}
		else if (!ret)
			return (FT_PING_TIMEOUT);

		ret = process_received_packet(socket_fd, send_time, context);
		switch (ret)
		{
			case FT_PING_OK:
				return (FT_PING_OK);
			case FT_PING_SIGINT:
				return (FT_PING_SIGINT);
			case FT_PING_ERROR:
				return (FT_PING_ERROR);
			default:
				continue;
		}
	}
	return (FT_PING_SIGINT);
}
