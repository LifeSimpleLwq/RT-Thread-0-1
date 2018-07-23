#include <rtthread.h>
#include <rthw.h>

#define	IDLE_THREAD_STACK_SIZE		512

extern	rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];

struct rt_thread idle;

ALIGN(RT_ALIGN_SIZE)

static rt_uint8_t rt_thread_stack[IDLE_THREAD_STACK_SIZE];

rt_ubase_t rt_idletask_ctr = 0;

/* 空闲线程是在做一些系统内存清理工作 */
void rt_thread_idle_entry(void *parameter)
{
	parameter = parameter;
	while(1)
	{
		rt_idletask_ctr ++;
	}
}

void rt_thread_idle_init(void)
{
	rt_thread_init(&idle,
						"idle_t",
						rt_thread_idle_entry,
						RT_NULL,
						rt_thread_stack,
						sizeof(rt_thread_stack),
						RT_THREAD_PRIORITY_MAX-1);
	//rt_list_insert_before(&(rt_thread_priority_table[RT_THREAD_PRIORITY_MAX-1]),&(idle.tlist));
	rt_thread_startup(&idle);
}











