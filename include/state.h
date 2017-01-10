#include <string.h>
#include <assert.h>
#include "common.h"

#ifndef __STATE_H__
#define __STATE_H__

typedef struct state_s state_t;

typedef enum
{
	state_errno_exit = 0,
	state_errno_entry,
	state_errno_event,
	state_errno_emptyf,
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
} state_meta_t;

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
	void 			*ctx;
	state_table_t 	*table;
};

static inline
void state_set_ctx(state_t *st, void *ctx)
{
	st->ctx = ctx;
}

static inline
void* state_get_ctx(state_t *st, void *ctx)
{
	return st->ctx;
}

static inline
state_meta_t* state_get_meta(state_t *st)
{
	state_table_t *table = st->table;
	return &table->meta[st->state];
}

static inline
state_ops_t* state_get_ops(state_t *st)
{
	state_meta_t *meta = state_get_meta(st);
	return &meta->ops;
}

static inline
int state_exchange(state_t *st, uint32_t state)
{
	state_ops_t *ops = state_get_ops(st);;
	if (ops->on_exit && ops->on_exit(st)) {
		st->errno = state_errno_exit;
		return -1;
	}

	st->state = state;
	ops = state_get_ops(st);

	if (ops->on_entry && ops->on_entry(st)) {
		st->errno = state_errno_entry;
		return -1;
	}
	
	return 0;
}

#define STATE_FINISH_FLAG 	0x1
#define STATE_STOP_FLAG		0x2

static inline
void state_stop(state_t *st)
{
	set_flag16(&st->flags, STATE_STOP_FLAG);
}

static inline
int state_is_stoped(state_t *st)
{
	return test_flag16(&st->flags, STATE_STOP_FLAG);
}

static inline
void state_finish(state_t *st)
{
	set_flag16(&st->flags, STATE_FINISH_FLAG);
}

static inline
int state_is_finished(state_t *st)
{
	return test_flag16(&st->flags, STATE_FINISH_FLAG);
}

static inline
int state_valid(state_t *st)
{
	if (state_is_finished(st)) {
		return 0;
	} else if (state_is_stoped(st)) {
		return 0;
	} else if (st->errno) {
		return 0;
	}

	return 1;
}

#define STATE_STOPED 	-1
#define STATE_RUNNING 	0
#define STATE_FINISHED 	1

static inline
int state_run(state_t *st, uint32_t ev)
{
	if (!state_valid(st)) {
		goto error;
	}

	state_ops_t *ops = state_get_ops(st);
	if (!ops->on_event) {
		st->errno = state_errno_emptyf;
		goto error;
	}
	uint32_t new_state = ops->on_event(st, ev);
check:
	if (state_is_finished(st)) {
		return STATE_FINISHED;
	} else if (state_is_stoped(st)) {
	   	return STATE_STOPED;
	} else if (new_state != st->state) {
		if (state_exchange(st, new_state)) {
			goto error;
		}
		assert(new_state == st->state);
		goto check;
	}

	return STATE_RUNNING;
error:
	state_stop(st);
	return STATE_STOPED;
}

static inline
void state_reset(state_t *st)
{
	memset(st, 0, offsetof(state_t, table));
}

static inline
int state_init(state_t *st, state_table_t *table)
{
	memset(st, 0, sizeof(state_t));
	st->table = table;
	state_reset(st);

	return 0;
}

#endif //__STATE_H__
