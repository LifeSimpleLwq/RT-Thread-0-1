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
	
	rt_timer_init(&(thread->thread_timer),	/* ��̬��ʱ������ */
					thread->name,			/* ��ʱ��name */
					rt_thread_timeout,		/* ��ʱ���� */
					thread,					/* ��ʱ�����β� */
					0,						/* ��ʱʱ��*/
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
	
	/* �ı��߳�״̬ */
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
	/*  ���õ�ǰ���ȼ�Ϊ��ʼ�����ȼ� */
	thread->current_priority = thread->init_priority;
	thread->number_mask = 1L << thread->current_priority;
	
	/* �ı��̵߳�״̬Ϊ���� */
	thread->stat = RT_THREAD_SUSPEND;
	
	/* �ָ��߳� */
	rt_thread_resume(thread);
	
	if (rt_thread_self() != RT_NULL)
	{
		/* ϵͳ���� */
		rt_schedule();
	}
	return RT_EOK;
}

/* �ú������ڻָ�һ���̣߳�Ȼ����ŵ������б� */
rt_err_t rt_thread_resume(rt_thread_t thread)
{
	register rt_base_t temp;
	
	/* �����ָ����̱߳������ڹ���״̬�����򷵻ش����� */
	if ((thread->stat & RT_THREAD_STAT_MASK) != RT_THREAD_SUSPEND)
	{
		return -RT_ERROR;
	}
	
	temp = rt_hw_interrupt_disable();
	
	/* �ӹ������ɾ�� */
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
 * �̳߳�ʱ����
 * ���߳���ʱ���� or �ȴ�����Դ���� or ��ʱʱ���ú����ᱻ����
 *
 * @param parameter ��ʱ�������β�
 */
void rt_thread_timeout(void *parameter)
{
	struct rt_thread *thread;
	
	thread = (struct rt_thread *)parameter;
	
	/* ���ô�����Ϊ��ʱ */
	thread->error = -RT_ETIMEOUT;
	
	/* ���̴߳ӹ����б���ɾ�� */
	rt_list_remove(&(thread->tlist));
	
	/* ���̲߳��뵽�����б� */
	rt_schedule_insert_thread(thread);
	
	rt_schedule();
}

/**  
 *	�ú������õ�ǰ�߳�˯��һ��ʱ�䣬��λtick
 */
rt_err_t rt_thread_sleep(rt_tick_t tick)
{
	register rt_base_t temp;
	struct rt_thread *thread;
	
	temp = rt_hw_interrupt_disable();
	
	/* ��ȡ��ǰ�̵߳��߳̿��ƿ� */
	thread = rt_current_thread;
	
	/* �����߳� */
	rt_thread_suspend(thread);
	
	/* �����̶߳�ʱ���ĳ�ʱʱ�� */
	rt_timer_control(&(thread->thread_timer), RT_TIMER_CTRL_SET_TIME, &tick);
	
	/* ������ʱ�� */
	rt_timer_start(&(thread->thread_timer));
	
	rt_hw_interrupt_enable(temp);
	
	rt_schedule();
	
	return RT_EOK;
}

rt_err_t rt_thread_suspend(rt_thread_t thread)
{
	register rt_base_t temp;
	
	/* ֻ�о������̲߳��ܱ����𣬷����˳����ش����� */
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






























