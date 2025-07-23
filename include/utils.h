#pragma once

#include "commons.h"

/*	UTILS - TIME */
void				get_timestamp(struct timeval *timestamp);
double				get_elapsedtime_ms(struct timeval *start, \
										struct timeval *end);

/*	UTILS - CHECKSUM */
unsigned short		checksum(void *buffer, int length);
