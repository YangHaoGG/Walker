#include "module.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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
	while ((read = getdelim(&name, &size, '\n', file)) != -1) {
		name[read - 1] = 0;
		printf("1\n");
		if (!dlopen(name, RTLD_NOW)) {
			printf("dlopen %s failed:%s", name, strerror(errno));
		}
	}

	printf("2\n");
	call_list();
	return 0;
}
