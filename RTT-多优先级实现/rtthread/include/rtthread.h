#ifndef __RT_THREAD_H__
#define __RT_THREAD_H__

#include <rtdef.h>
#include <rtconfig.h>
#include <rtservice.h>

extern	struct rt_thread *rt_current_thread;
/*********************************
															
*********************************/
rt_err_t	rt_thread_init(struct rt_thread *thread,
								const	char 			*name,
								void (*entry)(void *parameter),
								void 				*parameter,
								void 				*stack_start,
								rt_uint32_t	stack_size,
								rt_uint8_t	priority);
rt_err_t rt_thread_startup(rt_thread_t thread);
rt_err_t rt_thread_resume(rt_thread_t thread);
								
rt_thread_t rt_thread_self(void);								
void rt_thread_delay(rt_tick_t tick);
/*********************************
															
*********************************/
rt_uint8_t	*rt_hw_stack_init(void 	*tentry,
										void  *parameter,
										rt_uint8_t *stack_addr);

/*********************************
															
*********************************/
void rt_system_scheduler_init(void);
void rt_system_scheduler_start(void);
void rt_schedule(void);
void rt_schedule_insert_thread(struct rt_thread *thread);										
void rt_schedule_remove_thread(struct rt_thread *thread);
										
/*********************************
															
*********************************/										
char *rt_strncpy(char *dst,const char *str,rt_ubase_t n);	
int __rt_ffs(int value);
										
/*********************************
															
*********************************/	
void rt_object_init(struct rt_object 				*object,
						  enum rt_object_class_type 	type,
						  const char 						*name);										

void rt_thread_idle_init(void);
						  
void rt_tick_increase(void);					  
void rt_interrupt_enter(void);
void rt_interrupt_leave(void);
						  
#endif
