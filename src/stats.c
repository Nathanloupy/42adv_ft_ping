#include "commons.h"

/*
 * Updates the statistics for a received packet
 * - Increments the packet received count
 * - Adds the elapsed time to the total time
 * - Updates the minimum and maximum times
*/
void update_stats(t_ping_context *context, double elapsed_time)
{
	context->stats.packets_received++;
	context->stats.total_time += elapsed_time;
	context->stats.sum_squared_time += elapsed_time * elapsed_time;
	if (elapsed_time < context->stats.min_time)
		context->stats.min_time = elapsed_time;
	if (elapsed_time > context->stats.max_time)
		context->stats.max_time = elapsed_time;
}

/*
 * Calculates and prints the ping statistics
 * - Packet loss percentage
 * - Round-trip time statistics (min/avg/max/stddev)
 */
void	print_statistics(t_ping_context *context)
{
	double	packet_loss = 0.0;
	double	avg_time = 0.0;
	double	stddev = 0.0;

	if (context->stats.packets_sent > 0)
		packet_loss = 100.0 * (context->stats.packets_sent - context->stats.packets_received) / context->stats.packets_sent;
	
	if (context->stats.packets_received > 0)
	{
		avg_time = context->stats.total_time / context->stats.packets_received;
		
		if (context->stats.packets_received > 1)
		{
			double variance = (context->stats.sum_squared_time / context->stats.packets_received) - (avg_time * avg_time);
			if (variance > 0)
				stddev = sqrt(variance);
		}
	}
	
	printf("--- %s ping statistics ---\n", context->dest_host);
	printf("%d packets transmitted, %d packets received, %d%% packet loss\n",
		   context->stats.packets_sent, context->stats.packets_received, (int)packet_loss);
	
	if (context->stats.packets_received > 0)
		printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
			   context->stats.min_time, avg_time, context->stats.max_time, stddev);
}
