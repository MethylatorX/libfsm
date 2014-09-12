/* $Id$ */

#ifndef RE_COMP_H
#define RE_COMP_H

#include <re/re.h>

struct fsm *
comp_literal(int (*f)(void *opaque), void *opaque,
	enum re_flags flags, struct re_err *err);

struct fsm *
comp_glob(int (*f)(void *opaque), void *opaque,
	enum re_flags flags, struct re_err *err);

struct fsm *
comp_simple(int (*f)(void *opaque), void *opaque,
	enum re_flags flags, struct re_err *err);

#endif

