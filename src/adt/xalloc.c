/* $Id$ */

#include <stdlib.h>
#include <string.h>

#include <adt/xalloc.h>

char *
xstrdup(const char *s)
{
	char *new;

	new = malloc(strlen(s) + 1);
	if (new == NULL) {
		return NULL;
	}

	return strcpy(new, s);
}

