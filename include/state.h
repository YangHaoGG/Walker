#include "list.h"

#ifndef __STATE_H__
#define __STATE_H__

typedef struct state_s state_t;

typedef enum
{
	state_errno_exit = 0,
	state_errno_entry,
	state_errno_event,
	state_errno_max
} state_errno_e;

typedef struct state_ops_s
{
	int32_t 	(*on_entry)(state_t *st);
	int32_t 	(*on_exit)(state_t *st);
	uint32_t 	(*on_event)(state_t *st, uint32_t ev);
} state_ops_t;

typedef struct state_meta_s
{
	const char *name;
	state_ops_t ops;
} state_meta_e;

typedef struct state_table_s
{
	uint32_t 		cnt;
	state_meta_t 	meta[0];
} state_table_t;

struct state_s
{
	uint32_t 		state;
	uint16_t 		flags;
	uint16_t 		errno;
	state_ops_t 	*ops;
	state_table_t 	*table;
	void 			*ctx;
};

static inline
void state_set_ctx(state_t *st, void *ctx)
{
	st->ctx = ctx;
}

static inline
void state_get_ctx(state_t *st, void *ctx)
{
	return st->ctx;
}

static inline
state_ops_t* state_get_ops(state_t *st, uint32_t state)
{
	state_table_t *table = st->table;
	return &table->meta[st->state].ops;
}

static inline
int state_exchange(state_t *st, uint32_t state)
{
	state_ops_t *ops = st->ops;
	if (ops->on_exit && ops->on_exit(st)) {
		st->errno = state_errno_exit;
		return -1;
	}

	st->ops = state_get_ops(st->table, state);
	st->state = state;

	if (ops->on_entry && ops->on_entry(st)) {
		st->errno = state_errno_entry;
		return -1;
	}
	
	return 0;
}

static inline
int state_check(state_t *st)
{
	if (st->errno) {
		return -1;
	}
	return 0;
}

static inline
void state_error(st)
{
	return
}

static inline
int state_run(state_t *st, uint32_t ev)
{
	if (state_check(st)) {
		goto error;
	}

	uint32_t new_state = st->ops->on_event(st, ev);
	if (new_state != st->state) {
		if (state_exchange(st, new_state)) {
			goto error;
		}
	}
	return 0;
error:
	state_error(st);
	return -1;
}

static inline
int state_init(state_t *st, state_table_t *table)
{
	uint32_t i = 0;
	while (i < table->cnt) {
		if (table->meta[i].ops.on_event == NULL) {
			return -1;
		}
	}
	memset(st, 0, sizeof(state_t));
	st->state = 0;
	st->table = table;
	st->ops = state_get_ops(st);

	return 0;
}

#endif //__STATE_H__
