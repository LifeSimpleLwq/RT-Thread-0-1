#include <rtthread.h>
#include <rthw.h>

static rt_tick_t rt_tick = 0;
extern rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];
extern rt_uint32_t rt_thread_ready_priority_group;

#if 0
void rt_tick_increase(void)
{
	rt_ubase_t i;
	struct rt_thread *thread;
	rt_tick ++;

	for(i = 0; i< RT_THREAD_PRIORITY_MAX; i++)
	{
		thread = rt_list_entry(rt_thread_priority_table[i].next,
									  struct rt_thread,
									  tlist);
	   if (thread->remaining_tick > 0)
		{
			thread->remaining_tick --;
			if(thread->remaining_tick ==0)
			{
				rt_thread_ready_priority_group |= thread->number_mask;
			}
		}
	}

	rt_schedule();
}
#else 
void rt_tick_increase(void)
{
	++rt_tick;
	
	rt_timer_check();
}	

#endif

rt_tick_t rt_tick_get(void)
{
    /* return the global tick */
    return rt_tick;
}





