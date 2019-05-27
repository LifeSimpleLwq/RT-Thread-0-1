#include <rtthread.h>
#include <rthw.h>

extern rt_uint32_t rt_thread_ready_priority_group;

rt_err_t rt_thread_init(struct rt_thread 	*thread,
												const char 				*name,
												void (*entry)(void *parameter),
												void 							*parameter,
												void 							*stack_start,
												rt_uint32_t 			stack_size,
												rt_uint8_t  			priority)
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
	
	thread->init_priority   = priority;
	thread->current_priority = priority;
	thread->number_mask = 0;
	
	// 错误码和状态
	thread->error = RT_EOK;
	thread->stat  = RT_THREAD_INIT; 
	
	return RT_EOK;
}

void rt_thread_delay(rt_tick_t tick)
{
#if 0
	struct rt_thread *thread;
	
	thread = rt_current_thread;
	
	thread->remaining_tick = tick;
	
	rt_schedule();
#else 
	register rt_base_t temp;
	struct rt_thread *thread;
	
	temp = rt_hw_interrupt_disable();
	
	thread = rt_current_thread;
	thread->remaining_tick = tick;
	
	thread->stat = RT_THREAD_SUSPEND;
	rt_thread_ready_priority_group &= ~thread->number_mask;
	
	rt_hw_interrupt_enable(temp);
	
	rt_schedule();
#endif 	
}
rt_err_t rt_thread_resume(rt_thread_t thread)
{
	register rt_base_t temp;
	
	// 即将恢复的线程必须为挂起态，否则返回错误码
	if ((thread->stat & RT_THREAD_STAT_MASK) != RT_THREAD_SUSPEND)
	{
		return -RT_ERROR;
	}
	
	temp = rt_hw_interrupt_disable();
	
	// 从挂起队列删除
	rt_list_remove(&(thread->tlist));
	
	rt_hw_interrupt_enable(temp);
	
	// 插入就绪队列
	rt_schedule_insert_thread(thread);
	
	return RT_EOK;
}

// 返回当前线程
rt_thread_t rt_thread_self()
{
	return rt_current_thread;
}

/**
*	启动一个线程并将其放到系统得就绪列表中	
*/
rt_err_t rt_thread_startup(rt_thread_t thread)
{
	// 设置当前优先级
	thread->current_priority = thread->init_priority;
	thread->number_mask = 1L << thread->current_priority;
	
	// 改变线程状态为挂起状态
	thread->stat = RT_THREAD_SUSPEND;
	
	//  恢复线程
	rt_thread_resume(thread);
	
	if (rt_thread_self() != RT_NULL)
	{
		rt_schedule();
	}
	
	return RT_EOK;
}


