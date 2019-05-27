#include "rtthread.h"

rt_err_t rt_thread_init(struct rt_thread *thread,
												const char *name,
												void (*entry)(void *parameter),
												void 				*parameter,
												void 				*stack_start,
												rt_uint32_t stack_size)
{
	rt_object_init((rt_object_t)thread, RT_Object_Class_Thread, name);
	
	rt_list_init(&(thread->tlist));
	
	thread->entry = (void *)entry;
	thread->parameter = parameter;
	
	thread->stack_addr = stack_start;
	thread->stack_size = stack_size;
	
	thread->sp = 
	(void *)rt_hw_stack_init( thread->entry,
														thread->parameter,
	(void *)((char *)thread->stack_addr + thread->stack_size -4) );
	
	return RT_EOK;
}





