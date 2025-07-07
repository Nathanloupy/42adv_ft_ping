#pragma once

#include "commons.h"

/*	UTILS - TIME */
void				gettimestamp(struct timeval *timestamp);
double				getelapsedtime_ms(struct timeval *start, \
										struct timeval *end);

/*	UTILS - CHECKSUM */
unsigned short		checksum(void *b, int len);