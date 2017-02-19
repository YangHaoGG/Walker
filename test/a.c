#include "module.h"
#include <stdio.h>

void mycb(void *x)
{
	printf("hello world\n");
}

__attribute__((constructor)) void __init__()
{
	printf("init\n");
	register_callback(mycb, 0);
}
