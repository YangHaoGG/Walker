#ifndef __IO_H__
#define __IO_H__

typedef enum
{
	IO_OPEN,
	IO_DATA,
	IO_FINISH,
	IO_CLOSE,
	IO_MAX
} io_event_e;


struct io_param
{
	int type;
};

struct io_module
{
	int id;
	char name[256];

	int module_private_size;
	int io_private_size;

	struct io_operation op;
	struct list_head list;
	struct list_head node;
	//alloc
	//free
	char private[0];
};

struct io_event
{
	int (*on_active_opened)(struct io *io);
	int (*on_passive_opened)(struct io *io);
	int (*on_active_closed)(struct io *io);
	int (*on_passive_closed)(struct io *io);
	int (*on_data_received)(struct io *io);
	int (*on_data_sendover)(struct io *io);
};

struct io
{
	struct io_module *module;
	struct io_operation *op;
	struct io_event *event;
	struct list_head node;
	char private[0];
};

struct io_operation
{
	struct io* (*open)(io_param *param);
	int (*close)(struct io *io);

	int (*read)(struct io *io, char *buf, int n);
	int (*write)(struct io *io, char *buf, int n);

};

#endif
