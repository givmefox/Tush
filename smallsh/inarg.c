#include "smallsh.h"

static char special [] = {' ', '\t', '&', ';', '\n', '\0'};

int inarg(char c)
{
	char *wrk;

	for(wrk = special; *wrk; wrk++)
	{
		if(c==*wrk)
			return (0);
	}

	return (1);
} /* End of inarg */
