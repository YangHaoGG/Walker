/**
 * floor: 	a plateform of data service, this point is DATA IS SERVICE.
 *
 * file: 	the local functoin start with _fr, the global functon start with fr.
 *
 * Author: 	yhao
 * Time:	2017-03-13
 *
 */

#include <stdio.h>
#include "log.h"
#include "config.h"

int _fr_parse_options(int argv, char *argc[], void *options[]);
int _fr_init_config(struct config *config, void *options[]);
int _fr_apply_config(struct config *config);
int _fr_run_monitor(struct config *config);

struct config config = {0};

int main(int argv, char *argc[])
{
	void *options[1];
	if (_fr_parse_options(argv, argc, options)) {
		return -1;
	}

	if (_fr_init_config(&config, options)) {
		return -1;
	}

	if (_fr_apply_config(&config)) {
		return -1;
	}

	if (_fr_run_monitor(&config)) {
		return -1;
	}

	return 0;
}

int _fr_parse_options(int argv, char *argc[], void *options[])
{
	fr_debug("parse options\n");
	return 0;
}

int _fr_init_config(struct config *config, void *options[])
{
	fr_debug("fine\n");
	return 0;
}

int _fr_apply_config(struct config *config)
{
	fr_debug("fine\n");
	return 0;
}

int _fr_run_monitor(struct config *config)
{
	fr_debug("fine\n");
	return 0;
}
