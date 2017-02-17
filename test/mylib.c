#include "mylib.h"
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

int nops = 0;
struct operation g_ops[128] = {0};

void register_callback(void (*cb)(void *), void *ctx)
{
	printf("register callback cb:%p ctx:%p\n", cb, ctx);
	if (nops < 128) {
		g_ops[nops].cb = cb;
		g_ops[nops].ctx = ctx;
		nops++;
	}
}

int main()
{
	FILE *file = fopen("./list.txt", "r");
	if (file == NULL) {
		printf("fopen failed\n");
		return -1;
	}

	char *name = 0;
	size_t size = 0;
	ssize_t read = 0;
	char buf[512];
	while ((read = getdelim(&name, &size, '\n', file)) != -1) {
		name[read - 1] = 0;
		snprintf(buf, 512, "/home/ubuntu/work/Walker/test/%s", name);
		printf("1\n");
		if (!dlopen(buf, RTLD_NOW)) {
			printf("dlopen %s failed:%s", buf, strerror(errno));
		}
	}

		printf("2\n");
	int i = 0;
	while (i < nops) {
		g_ops[i].cb(g_ops[i].ctx);
		i++;
	}
	return 0;
}
