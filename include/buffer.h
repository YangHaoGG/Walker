#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "operator.h"

struct buffer
{
	struct operator *ops;

	uint64_t ref;
	size_t length;
	size_t capability;
	size_t rpos;
	size_t wpos;

	char *address;
	char data[0];
};

struct buffer_node
{
	struct buffer buffer;
	struct list_head node;
	char data[0];
};

#endif
