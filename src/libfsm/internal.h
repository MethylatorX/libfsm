/* $Id$ */

#ifndef INTERNAL_H
#define INTERNAL_H

#include <limits.h>


struct fsm_edge {
	struct fsm_state *state;
	struct trans_list *trans;
};

/* TODO: +2 for SOL, EOL */
/* TODO: +lots for FSM_EDGE_* */
#define FSM_EDGE_MAX UCHAR_MAX

struct fsm_state {
	unsigned int id;
	void *opaque;

	int end;

	struct fsm_edge edges[FSM_EDGE_MAX];
};


/* global registry of all states */
struct state_list {
	struct fsm_state state;
	struct state_list *next;
};

/*
 * Note that these are used to index state.edges[] (as well as the more usual
 * index of a character cast to unsigned char).
 * TODO: due to get rid of all of these
 */
enum fsm_edge_type {
	FSM_EDGE_EPSILON = FSM_EDGE_MAX,
	FSM_EDGE_ANY,
	FSM_EDGE_LITERAL,
	FSM_EDGE_LABEL
};

union trans_value {
	char literal;
	char *label;
};

/* global registry of all transitions */
struct trans_list {
	enum fsm_edge_type type;
	union trans_value u;

	struct trans_list *next;
};

struct fsm {
	struct state_list *sl;
	struct trans_list *tl;
	struct fsm_state *start;
	struct fsm_options options;
};

#endif

