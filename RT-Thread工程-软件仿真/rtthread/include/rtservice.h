#ifndef __RT_SERVICE_H__
#define __RT_SERVICE_H__

#include "rtthread.h"

// 初始化链表
rt_inline void rt_list_init(rt_list_t *l)
{
	l->next = l->prev = l;
}

// 在双向链表头部 后面 插入一个节点
rt_inline void rt_list_insert_after(rt_list_t *l, rt_list_t *n)
{
	l->next->prev = n;	
	n->next = l->next;
	
	l->next = n;
	n->prev = l;
}

// 在双向链表头部 前面 插入一个节点
rt_inline void rt_list_insert_before(rt_list_t *l, rt_list_t *n)
{
	n->next = l;
	n->prev = l->prev;
	
	l->prev->next = n;	
	l->prev = n;
}

// 从双向链表删除一个节点
rt_inline void rt_list_remove(rt_list_t *n)
{
	n->next->prev = n->prev;
	n->prev->next = n->next;
	
	n->next = n->prev = n;
}

#endif /* __RT_SERVICE_H__ */
