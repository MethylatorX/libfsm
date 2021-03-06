#include <stdio.h>
#include <string.h>

#include <fsm/fsm.h>
#include <fsm/bool.h>
#include <fsm/out.h>

#include <re/re.h>

int main(void) {
	struct fsm *fsm;
	struct fsm_state *start;
	char s[BUFSIZ];

	fsm = fsm_new();
	if (fsm == NULL) {
		perror("fsm_new");
		return 1;
	}

	start = fsm_addstate(fsm);
	if (start == NULL) {
		perror("fsm_addtate");
		return 1;
	}

	fsm_setstart(fsm, start);

	while (fgets(s, sizeof s, stdin) != NULL) {
		struct fsm *r;
		struct re_err e;
		const char *p = s;

		s[strcspn(s, "\n")] = '\0';

		fprintf(stderr, "%s\n", s);

		r = re_comp(RE_LITERAL, re_sgetc, &p, 0, &e);
		if (r == NULL) {
			re_perror(RE_LITERAL, &e, NULL, s);
			return 1;
		}

		fsm = fsm_union(fsm, r);
		if (fsm == NULL) {
			perror("fsm_union");
			return 1;
		}
	}

	if (-1 == fsm_minimise(fsm)) {
		perror("fsm_minimise");
		return 1;
	}

	{
		static const struct fsm_outoptions o_defaults;
		struct fsm_outoptions o = o_defaults;

		o.anonymous_states  = 1;
		o.consolidate_edges = 1;

		fsm_print(fsm, stdout, FSM_OUT_DOT, &o);
	}

	return 0;
}

