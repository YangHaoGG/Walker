#include <stddef.h>
#include <stdint.h>

#ifndef __COMMON_H__
#define __COMMON_H__

static inline
void set_flag(uint32_t *flags, uint32_t flag)
{
	*flags |= flag;
}

static inline
uint32_t test_flag(uint32_t *flags, uint32_t flag)
{
	return *flags & flag;
}

static inline
void clear_flag(uint32_t *flags, uint32_t flag)
{
	*flags &= ~flag;
}

static inline
void set_flag16(uint16_t *flags, uint16_t flag)
{
	*flags |= flag;
}

static inline
uint16_t test_flag16(uint16_t *flags, uint16_t flag)
{
	return *flags & flag;
}

static inline
void clear_flag16(uint16_t *flags, uint16_t flag)
{
	*flags &= ~flag;
}
#endif
