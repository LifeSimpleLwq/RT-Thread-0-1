#include <rtthread.h>
#include <rthw.h>

extern rt_uint32_t rt_thread_ready_priority_group;

rt_err_t rt_thread_init(struct rt_thread *thread,
						const char 			*name,
						void (*entry)(void *parameter),
						void 				*parameter,
						void 				*stack_start,
						rt_uint32_t	stack_size,
						rt_uint8_t	priority)
{
	rt_object_init((rt_object_t)thread,RT_Object_Class_Thread,name);

	rt_list_init(&(thread->tlist));
	
	thread->entry	  = (void *)entry;
	thread->parameter = parameter;

	thread->stack_addr  = stack_start;
	thread->static_size = stack_size;

	thread->sp = 
	(void *)rt_hw_stack_init( thread->entry,
							  thread->parameter,
	(void *)((char *)thread->stack_addr + thread->static_size -4));
	
	thread->init_priority = priority;
	thread->current_priority = priority;
	thread->number_mask = 0;
	
	thread->error = RT_EOK;
	thread->stat  = RT_THREAD_INIT;
	
	rt_timer_init(&(thread->thread_timer),	/* 静态定时器对象 */
					thread->name,			/* 定时器name */
					rt_thread_timeout,		/* 超时函数 */
					thread,					/* 超时函数形参 */
					0,						/* 延时时间*/
					RT_TIMER_FLAG_ONE_SHOT);
	
	return RT_EOK;
}
#if 0
void rt_thread_delay(rt_tick_t tick)
{
	register rt_base_t temp;
	struct rt_thread *thread;
	
	temp = rt_hw_interrupt_disable();
	
	thread = rt_current_thread;
	thread->remaining_tick = tick;
	
	/* 改变线程状态 */
	thread->stat = RT_THREAD_SUSPEND;
	rt_thread_ready_priority_group &= ~thread->number_mask;
	
	rt_hw_interrupt_enable(temp);
	
	rt_schedule();
}
#else 
rt_err_t rt_thread_delay(rt_tick_t tick)
{
	return rt_thread_sleep(tick);
}	
#endif

rt_err_t rt_thread_startup(rt_thread_t thread)
{
	/*  设置当前优先级为初始化优先级 */
	thread->current_priority = thread->init_priority;
	thread->number_mask = 1L << thread->current_priority;
	
	/* 改变线程的状态为挂起 */
	thread->stat = RT_THREAD_SUSPEND;
	
	/* 恢复线程 */
	rt_thread_resume(thread);
	
	if (rt_thread_self() != RT_NULL)
	{
		/* 系统调度 */
		rt_schedule();
	}
	return RT_EOK;
}

/* 该函数用于恢复一个线程，然后将其放到就绪列表 */
rt_err_t rt_thread_resume(rt_thread_t thread)
{
	register rt_base_t temp;
	
	/* 即将恢复的线程必须是在挂起状态，否则返回错误码 */
	if ((thread->stat & RT_THREAD_STAT_MASK) != RT_THREAD_SUSPEND)
	{
		return -RT_ERROR;
	}
	
	temp = rt_hw_interrupt_disable();
	
	/* 从挂起队列删除 */
	rt_list_remove(&(thread->tlist));
	
	rt_hw_interrupt_enable(temp);

	rt_schedule_insert_thread(thread);
   return RT_EOK;
}

rt_thread_t rt_thread_self(void)
{
	return rt_current_thread;
}

/**
 * 线程超时函数
 * 当线程延时到期 or 等待的资源可用 or 超时时，该函数会被调用
 *
 * @param parameter 超时函数的形参
 */
void rt_thread_timeout(void *parameter)
{
	struct rt_thread *thread;
	
	thread = (struct rt_thread *)parameter;
	
	/* 设置错误码为超时 */
	thread->error = -RT_ETIMEOUT;
	
	/* 将线程从挂起列表中删除 */
	rt_list_remove(&(thread->tlist));
	
	/* 将线程插入到就绪列表 */
	rt_schedule_insert_thread(thread);
	
	rt_schedule();
}

/**  
 *	该函数将让当前线程睡眠一段时间，单位tick
 */
rt_err_t rt_thread_sleep(rt_tick_t tick)
{
	register rt_base_t temp;
	struct rt_thread *thread;
	
	temp = rt_hw_interrupt_disable();
	
	/* 获取当前线程的线程控制块 */
	thread = rt_current_thread;
	
	/* 挂起线程 */
	rt_thread_suspend(thread);
	
	/* 设置线程定时器的超时时间 */
	rt_timer_control(&(thread->thread_timer), RT_TIMER_CTRL_SET_TIME, &tick);
	
	/* 启动定时器 */
	rt_timer_start(&(thread->thread_timer));
	
	rt_hw_interrupt_enable(temp);
	
	rt_schedule();
	
	return RT_EOK;
}

rt_err_t rt_thread_suspend(rt_thread_t thread)
{
	register rt_base_t temp;
	
	/* 只有就绪的线程才能被挂起，否则退出返回错误码 */
	if ((thread->stat & RT_THREAD_STAT_MASK) != RT_THREAD_READY)
	{
		return -RT_ERROR;
	}
	
	temp = rt_hw_interrupt_disable();
	
	thread->stat = RT_THREAD_SUSPEND;
	
	rt_schedule_remove_thread(thread);
	
	rt_timer_stop(&(thread->thread_timer));
	
	rt_hw_interrupt_enable(temp);
	
	return RT_EOK;
}






























