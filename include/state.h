#include "list.h"

#ifndef __STATE_H__
#define __STATE_H__

typedef void * state_t;

typedef enum
{
	state_errno_exit = 0,
	state_errno_entry,
	state_errno_event,
	state_errno_max
} state_errno_e;

typedef struct state_ops_s
{
	int32_t 	(*on_entry)(state_t st);
	int32_t 	(*on_exit)(state_t st);
	uint32_t 	(*on_event)(state_t st, uint32_t ev);
} state_ops_t;

typedef struct state_meta_s
{
	state_ops_t ops;
} state_meta_e;

typedef struct state_table_s
{
	uint32_t 	cnt;
} state_table_t;

typedef struct state_s
{
	uint32_t 		state;
	uint16_t 		flags;
	uint16_t 		errno;
	state_ops_t 	*ops;
	state_table_t 	*table;
	void 			*ctx;
} state_t;

static inline
state_ops_t* state_get_ops(state_t *st, uint32_t state)
{
	return NULL;
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
int state_error(st)
{
	return 0;
}

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

#endif //__STATE_H__
