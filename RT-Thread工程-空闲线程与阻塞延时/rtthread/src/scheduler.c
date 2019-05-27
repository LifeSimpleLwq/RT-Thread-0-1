#include "rtthread.h"
#include "rthw.h"
			
// PendSV中断服务函数执行标志
rt_uint32_t	rt_thread_switch_interrupt_flag;
// 用于存储上一个线程的SP指针						
rt_uint32_t	rt_interrupt_from_thread;
// 用于存储下一个线程的sp指针
rt_uint32_t	rt_interrupt_to_thread;			
			
/* 当前线程 */
struct rt_thread *rt_current_thread;
/* 线程就绪列表 */
rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];

// 初始化调度器
void rt_system_scheduler_init(void)
{
	register rt_base_t offset;
	
	for (offset = 0; offset < RT_THREAD_PRIORITY_MAX; offset++)
	{
		rt_list_init(&rt_thread_priority_table[offset]);
	}
	
	rt_current_thread = RT_NULL;
}

// 启动调度器
void rt_system_scheduler_start(void)
{
	register struct rt_thread *to_thread;
	
	// 手动指定第一个运行的线程
	to_thread = rt_list_entry(rt_thread_priority_table[0].next,
														struct rt_thread,
														tlist);
	rt_current_thread = to_thread;
					
	/* 切换到第一个线程，该函数在 context_rvds.S 中实现，
		 在rthw.h声明，用于实现第一次线程切换。
		 当一个汇编函数在C文件中调用的时候，如果有形参，
		 则执行的时候会将形参传入到CPU寄存器r0. */
	rt_hw_context_switch_to((rt_uint32_t)&to_thread->sp);
}

extern struct rt_thread rt_flag1_thread;
extern struct rt_thread rt_flag2_thread;

void rt_schedule(void)
{
	struct rt_thread *to_thread;
	struct rt_thread *from_thread;
	
#if 0	
	if (rt_current_thread == rt_list_entry(rt_thread_priority_table[0].next,
																					struct rt_thread,
																					tlist))
	{
		from_thread = rt_current_thread;
		to_thread = rt_list_entry(rt_thread_priority_table[1].next,
																					struct rt_thread,
																					tlist);
		rt_current_thread = to_thread;																			
	}
	else 
	{
		from_thread = rt_current_thread;
		to_thread = rt_list_entry(rt_thread_priority_table[0].next,
																					struct rt_thread,
																					tlist);
		rt_current_thread = to_thread;		
	}
#else 
	if (rt_current_thread == &idle)
	{
		if (rt_flag1_thread.remaining_tick == 0)
		{
			from_thread = rt_current_thread;
			to_thread = &rt_flag1_thread;
			rt_current_thread = to_thread;
		}
		else if (rt_flag2_thread.remaining_tick == 0)
		{
			from_thread = rt_current_thread;
			to_thread = &rt_flag2_thread;
			rt_current_thread = to_thread;
		}
		else 
		{
			return;
		}
	}
	else 
	{
		if (rt_current_thread == &rt_flag1_thread)
		{
			if (rt_flag2_thread.remaining_tick == 0)
			{
				from_thread = rt_current_thread;
				to_thread = &rt_flag2_thread;
				rt_current_thread = to_thread;
			}
			else if (rt_current_thread->remaining_tick != 0)
			{
				from_thread = rt_current_thread;
				to_thread = &idle;
				rt_current_thread = to_thread;
			}
		}
		else if (rt_current_thread == &rt_flag2_thread)
		{
			if (rt_flag1_thread.remaining_tick == 0)
			{
				from_thread = rt_current_thread;
				to_thread = &rt_flag1_thread;
				rt_current_thread = to_thread;
			}
			else if (rt_current_thread->remaining_tick != 0)
			{
				from_thread = rt_current_thread;
				to_thread = &idle;
				rt_current_thread = to_thread;
			}
		}
	}
#endif
	rt_hw_context_switch((rt_uint32_t)&from_thread->sp, (rt_uint32_t)&to_thread->sp);
}






