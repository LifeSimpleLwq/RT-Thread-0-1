#ifndef __RT_SERVICE_H__
#define __RT_SERVICE_H__

#define	rt_container_of(node,type,member)	\
		((type *)((char *)(node) - (unsigned long)(&((type *)0)->member)))
#define	rt_list_entry(node,type,member)	\
		rt_container_of(node,type,member)

		
rt_inline void rt_list_init(rt_list_t *l)
{
		l->next = l->prev = l;
}

rt_inline void rt_list_insert_after(rt_list_t *l,rt_list_t *n)
{
		l->next->prev = n;
		n->next = l->next;
	
		l->next = n;
		n->prev = l;
}
					
rt_inline void rt_list_insert_before(rt_list_t *l,rt_list_t *n)
{
		l->prev->next = n;
		n->prev = l->prev;
	
		n->next = l;
		l->prev = n;
}

rt_inline void rt_list_remove(rt_list_t *n)
{
		n->prev->next = n->next;
		n->next->prev = n->prev;
	
		n->next = n->prev = n;
}

rt_inline int rt_list_isempty(const rt_list_t *l)
{
    return l->next == l;
}

#endif
