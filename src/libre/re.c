/* $Id$ */

#include <assert.h>
#include <stdlib.h>
#include <errno.h>

#include <re/re.h>

#include <fsm/fsm.h>
#include <fsm/bool.h>
#include <fsm/exec.h>
#include <fsm/graph.h>

#include "form/comp.h"

int
re_flags(const char *s, enum re_flags *f)
{
	const char *p;

	assert(s != NULL);
	assert(f != NULL);

	*f = 0U;

	/* defaults */
	*f |= RE_ZONE;

	for (p = s; *p != '\0'; p++) {
		if (*p & RE_ANCHOR) {
			*f &= ~RE_ANCHOR;
		}

		switch (*p) {
		case 'i': *f |= RE_ICASE;   break;
		case 'g': *f |= RE_TEXT;    break;
		case 'm': *f |= RE_MULTI;   break;
		case 'r': *f |= RE_REVERSE; break;
		case 's': *f |= RE_SINGLE;  break;
		case 'z': *f |= RE_ZONE;    break;

		default:
			errno = EINVAL;
			return -1;
		}
	}

	return 0;
}

struct fsm *
re_new_empty(void)
{
	struct fsm *new;
	struct fsm_state *start;

	new = fsm_new();
	if (new == NULL) {
		return NULL;
	}

	start = fsm_addstate(new);
	if (start == NULL) {
		goto error;
	}

	fsm_setstart(new, start);

	return new;

error:

	fsm_free(new);

	return NULL;
}

struct fsm *
re_new_comp(enum re_form form, int (*getc)(void *opaque), void *opaque,
	enum re_flags flags, struct re_err *err)
{
	struct fsm *new;
	struct fsm *(*comp)(int (*getc)(void *opaque), void *opaque,
		enum re_flags flags, struct re_err *err);

	assert(getc != NULL);

	switch (form) {
	case RE_LITERAL: comp = comp_literal; break;
	case RE_GLOB:    comp = comp_glob;    break;
	case RE_SIMPLE:  comp = comp_simple;  break;

	default:
		if (err != NULL) {
			err->e = RE_EBADFORM;
		}
		return NULL;
	}

	new = comp(getc, opaque, flags, err);
	if (new == NULL) {
		return NULL;
	}

	/*
	 * All flags operators commute with respect to composition.
	 * That is, the order of application here does not matter;
	 * here I'm trying to keep these ordered for efficiency.
	 */

	if (flags & RE_REVERSE) {
		if (!fsm_reverse(new)) {
			goto error;
		}
	}

	if (flags & RE_ICASE) {
		if (!fsm_desensitise(new)) {
			goto error;
		}
	}

	return new;

error:

	fsm_free(new);

	return NULL;
}

struct fsm_state *
re_exec(const struct fsm *fsm, const char *s)
{
	assert(fsm != NULL);
	assert(s != NULL);

	return fsm_exec(fsm, fsm_sgetc, &s);
}

