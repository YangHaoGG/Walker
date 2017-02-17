#ifndef __OPERATOR_H__
#define __OPERATOR_H__

enum {
	base_operator_e = 0,
	buffer_operator_e,

	max_operator_e;
};

#define BASE_OPERATOR_MASK (1 << base_operator_e)
struct base_operator
{
	int (*init)(void *handle);
	int (*fini)(void *handle);

	//ref ops
	int (*get)(void *handle);
	int (*put)(void *handle);
};

#define BUFFER_OPERATOR_MASK ((1 << buffer_operator_e) | BASE_OPERATOR_MASK)
struct buffer_operator
{
	struct base_operator base_ops;

	ssize_t (*read)(void *handle, char *from, size_t size);
	ssize_t (*write)(void *handle, char *to, size_t size);

	ssize_t (*read_at)(void *handle, size_t off, char *from, size_t size);
	ssize_t (*write_at)(void *handle, size_t off, char *to, size_t szie);

	ssize_t (*delete_buffer)(void *handle, size_t at, size_t size);
	ssize_t (*insert_buffer)(void *handle, size_t at, size_t size);

	size_t (*length)(void *handle);
	size_t (*capability)(void *handle);
};

struct operator
{
	uint64_t mask;

	union {
		struct base_operator base_ops;
		struct buffer_operator buffer_ops;
	};
};

#endif
