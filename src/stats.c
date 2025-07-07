#include "commons.h"

/**
 * Calculates and prints the ping statistics
 * - Packet loss percentage
 * - Round-trip time statistics (min/avg/max/stddev)
 */
void print_statistics(t_ping_context *context)
{
	double packet_loss = 0.0;
	if (context->stats.packets_sent > 0)
		packet_loss = 100.0 * (context->stats.packets_sent - context->stats.packets_received) / context->stats.packets_sent;
	
	double avg_time = 0.0;
	double stddev = 0.0;
	
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
	
	printf("--- %s ping statistics ---\n", context->destination_ip);
	printf("%d packets transmitted, %d packets received, %d%% packet loss\n",
		   context->stats.packets_sent, context->stats.packets_received, (int)packet_loss);
	
	if (context->stats.packets_received > 0)
		printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
			   context->stats.min_time, avg_time, context->stats.max_time, stddev);
} 