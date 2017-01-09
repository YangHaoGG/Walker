#include "debug.h"

#ifndef __LIST_H__
#define __LIST_H__

typedef struct list_head
{
	struct list_head *next, *prev;
} list_head_t;

#define list_entry(pos, type, node) (type*)((char*)pos - (unsigned long)(&((type*)0->node)))

static inline
void __list_add(struct list_head *new, struct list_head *prev, struct list_head *next)
{
	new->prev = prev;
	prev->next = new;
	new->next = next;
	next->prev = new;
}

static inline
void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}

static inline 
void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}

static inline
void __list_del(struct list_head *prev, struct list_head *next)
{
	prev->next = next;
	next->prev = prev;
}

static inline
void list_del(struct list_head *node)
{
	__list_del(node->prev, node->next);
	node->prev = node->next = NULL;
}

static inline
void list_del_init(struct list_head *node)
{
	__list_del(node->prev, node->next);
	node->prev = node->next = node;
}

static inline
void list_move(struct list_head *node, struct list_head *to)
{
	__list_del(node);
	list_add(node, to);
}

static inline
void list_move_tail(struct list_head *node, struct list_head *to)
{
	__list_del(node);
	list_add_tail(node, to);
}

static inline
int list_empty(struct list_head *node)
{
	if (node->next && node->next == node) {
		return 1;
	}
	return 0;

}

static inline
int list_valid(struct list_head *node)
{
	return node->next != NULL;
}

#define list_for_each_entry(pos, head, member) \
	for (pos = list_entry(head->next, typeof(pos), member); \
		 head != &pos->member; \
		 pos = list_entry(pos->member.next, typeof(pos), member))

#define list_for_each_entry_safe(pos, n, head, member) \
	for (pos = list_entry(head->next, typeof(pos), member), \
		 n = list_entry(pos->member.next, typeof(pos), member); \
		 head != &pos->member; \
		 pos = n, \
		 n = list_entry(pos->member.next, typeof(pos), member)) 

#endif //__LIST_H__
