/*
 * Automatically generated from the files:
 *	src/fsm/parser.sid
 * and
 *	src/fsm/parser.act
 * by:
 *	sid
 */

/* BEGINNING OF HEADER */

#line 98 "src/fsm/parser.act"


	typedef struct lex_state * lex_state;
	typedef struct act_state * act_state;
	typedef struct fsm *       fsm;

	struct fsm *
	fsm_parse(FILE *f);

#line 23 "src/fsm/parser.h"

/* BEGINNING OF FUNCTION DECLARATIONS */

extern void p_fsm(fsm, lex_state, act_state);
/* BEGINNING OF TRAILER */

#line 359 "src/fsm/parser.act"

#line 32 "src/fsm/parser.h"

/* END OF FILE */
