#ifndef __RT_THREAD_H__
#define __RT_THREAD_H__

#include "rtdef.h"
#include "rtconfig.h"
#include "rtservice.h"

extern struct rt_thread *rt_current_thread;;

extern rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];


// scheduler.c
void rt_system_scheduler_init(void);
void rt_system_scheduler_start(void);

// cpuport.c
rt_uint8_t *rt_hw_stack_init(void					*tentry,
															void 				*parameter,
															rt_uint8_t 	*stack_addr);

// thread.c
rt_err_t rt_thread_init(struct rt_thread *thread,
												const char *name,
												void (*entry)(void *parameter),
												void 				*parameter,
												void 				*stack_start,
												rt_uint32_t stack_size);

// kservice.c
char *rt_strncpy(char *dst, const char *src, rt_ubase_t n);
												
// object.c
void rt_object_init(struct rt_object 	*object,
										enum rt_object_class_type type,
										const char *name);

#endif /* __RT_THREAD_H__ */
												
												
												
