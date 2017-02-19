#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct operation
{
	void (*cb)(void *);
	void *ctx;
};

static int nops = 0;
static struct operation g_ops[128] = {0};

void register_callback(void (*cb)(void *), void *ctx)
{
	printf("register callback cb:%p ctx:%p\n", cb, ctx);
	if (nops < 128) {
		g_ops[nops].cb = cb;
		g_ops[nops].ctx = ctx;
		nops++;
	}
}

void call_list()
{
	int i = 0;
	while (i < nops) {
		g_ops[i].cb(g_ops[i].ctx);
		i++;
	}
}

