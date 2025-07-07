#include "commons.h"

volatile sig_atomic_t keep_running = 1;

/*
 * Signal handler for Ctrl+C
*/
void signal_handler(int sig)
{
	if (sig == SIGINT)
		keep_running = 0;
} 