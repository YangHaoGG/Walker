/**
 *
 *
 */

#ifndef __FLR_LOG_H__
#define __FLR_LOG_H__

#ifdef DEBUG
	#define fr_debug printf
#else
	#define fr_debug(fmt, ...)
#endif //DEBUG

#define fr_log(fmt, ...)
#define fr_error(fmt, ...)

#endif
