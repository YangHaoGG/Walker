#include "state.h"
#include <stdio.h>

uint32_t on_h(state_t *st, uint32_t ev)
{
	printf("on %c\n", (char)ev);
	if (ev == 'H') {
		return 1;
	}
	return 6;
}

uint32_t on_e(state_t *st, uint32_t ev)
{
	printf("on %c\n", (char)ev);
	if (ev == 'e') {
		return 2;
	}
	return 6;
}

uint32_t on_l(state_t *st, uint32_t ev)
{
	printf("on %c\n", (char)ev);
	if (ev == 'l') {
		return 3;
	}
	return 6;
}

uint32_t on_l1(state_t *st, uint32_t ev)
{
	printf("on %c\n", (char)ev);
	if (ev == 'l') {
		return 4;
	}
	return 6;
}

uint32_t on_o(state_t *st, uint32_t ev)
{
	printf("on %c\n", (char)ev);
	if (ev == 'o') {
		return 5;
	}
	return 6;
}

int32_t on_finish(state_t *st)
{
	printf("on finish\n");
	state_finish(st);
	return 0;
}

int32_t on_failed(state_t *st)
{
	printf("on failed\n");
	state_stop(st);
	return 0;
}

typedef struct hello_state_table_s
{
	state_table_t ori;
	state_meta_t m[7];
} hello_state_table_t;

hello_state_table_t char_set = {
	.ori = {
		.cnt = 7,
	},
	.m = {
		[0] = {
			.name = "H",
			.ops = { .on_event = on_h,},
		},
		[1] = {
			.name = "e",
			.ops = { .on_event = on_e,},
		},
		[2] = {
			.name = "l",
			.ops = { .on_event = on_l,},
		},
		[3] = {
			.name = "l",
			.ops = { .on_event = on_l1,},
		},
		[4] = {
			.name = "o",
			.ops = { .on_event = on_o,},
		},
		[5] = {
			.name = "finish",
			.ops = { .on_entry = on_finish,},
		},
		[6] = {
			.name = "failed", 
			.ops = { .on_entry = on_failed,},
		},
	},
};

int main()
{
	state_t st;
	state_init(&st, &char_set.ori);
	char buf[512];
	while (scanf("%s", buf)) {
		state_reset(&st);
		char *p = buf;
		while (*p) {
			int ret = state_run(&st, *p);
			if (ret == STATE_STOPED) {
				break;
			}
			if (ret == STATE_FINISHED) {
				printf("get Hello\n");
			}
			p++;
		}
	}
	return 0;
}
