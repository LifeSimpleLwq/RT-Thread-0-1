#include "rtthread.h"
#include "rthw.h"

// 线程优先级表
rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];

// 线程就绪优先级组
rt_uint32_t rt_thread_ready_priority_group;
rt_uint32_t rt_current_priority;

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
#if 0
	register rt_base_t offset;
	
	for (offset = 0; offset < RT_THREAD_PRIORITY_MAX; offset++)
	{
		rt_list_init(&rt_thread_priority_table[offset]);
	}
	
	rt_current_thread = RT_NULL;
#else 
	register rt_base_t offset;
	
	// 线程优先级初始化
	for (offset = 0; offset < RT_THREAD_PRIORITY_MAX; offset++)
	{
		rt_list_init(&rt_thread_priority_table[offset]);
	}
	
	// 初始化当前优先级为空闲线程
	rt_current_priority = RT_THREAD_PRIORITY_MAX - 1;
	
	// 初始化当前线程控制块指针
	rt_current_thread = RT_NULL;
	
	// 初始化线程就绪优先级组
	rt_thread_ready_priority_group = 0;
#endif
}

// 启动调度器
void rt_system_scheduler_start(void)
{
#if 0	
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
#else 
	register struct rt_thread *to_thread;
	register rt_ubase_t highest_ready_priority;
	
	// 获取就绪的最高优先级的线程
	highest_ready_priority = __rt_ffs(rt_thread_ready_priority_group) - 1;
	
	to_thread = rt_list_entry(rt_thread_priority_table[highest_ready_priority].next,
														struct rt_thread,
														tlist);
	
	rt_current_thread = to_thread;
	
	rt_hw_context_switch_to((rt_uint32_t)&to_thread->sp);
#endif
}

void rt_schedule(void)
{
	rt_base_t level;
	register rt_ubase_t highest_ready_priority;
	struct rt_thread *to_thread;
	struct rt_thread *from_thread;
	
	level = rt_hw_interrupt_disable();
	
	highest_ready_priority = __rt_ffs(rt_thread_ready_priority_group) - 1;
	
	to_thread = rt_list_entry(rt_thread_priority_table[highest_ready_priority].next,
														struct rt_thread,
														tlist);
	
	if (to_thread != rt_current_thread)
	{
		rt_current_priority = (rt_uint8_t)highest_ready_priority;
		from_thread = rt_current_thread;
		rt_current_thread = to_thread;
		
		rt_hw_context_switch((rt_uint32_t)&from_thread->sp,
												 (rt_uint32_t)&to_thread->sp);
		
		rt_hw_interrupt_enable(level);
	}
		
	rt_hw_interrupt_enable(level);
	
	rt_hw_context_switch((rt_uint32_t)&from_thread->sp, (rt_uint32_t)&to_thread->sp);
}

// 调度器插入线程
void rt_schedule_insert_thread(struct rt_thread *thread)
{
	register rt_base_t temp;
	
	temp = rt_hw_interrupt_disable();
	
	thread->stat = RT_THREAD_READY;
	
	rt_list_insert_before(&(rt_thread_priority_table[thread->current_priority]),
												&(thread->tlist));
	
	// 设置线程就绪优先级组中对应的位
	rt_thread_ready_priority_group |= thread->number_mask;
	
	rt_hw_interrupt_enable(temp);
}

// 调度器删除线程
void rt_schedule_remove_thread(struct rt_thread *thread)
{
	register rt_base_t temp;
	
	temp = rt_hw_interrupt_disable();
	
	rt_list_remove(&(thread->tlist));
	
	// 将线程就绪优先级组的对应的位位清除
//	if (rt_list_isempty(&(rt_thread_priority_table[thread->current_priority])))
//	{
		rt_thread_ready_priority_group &= ~thread->number_mask;
//	}
	
	rt_hw_interrupt_enable(temp);
}

