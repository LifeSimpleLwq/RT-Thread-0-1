#include "rtthread.h"

ALIGN(RT_ALIGN_SIZE)

rt_uint8_t	rt_flag1_thread_stack[512];
rt_uint8_t	rt_flag2_thread_stack[512];


struct rt_thread rt_flag1_thread;
struct rt_thread rt_flag2_thread;

rt_uint32_t	flag1;
rt_uint32_t flag2;


void delay(rt_uint32_t count)
{
	for(; count != 0; count--);
}

void flag1_thread_entry(void *p_arg)
{
	while (1)
	{
		flag1 = 1;
		delay(100);
		flag1 = 0;
		delay(100);
	}
}

void flag2_thread_entry(void *p_arg)
{
	while (1)
	{
		flag2 = 1;
		delay(100);
		flag2 = 0;
		delay(100);
	}
}

int main(void)
{
	rt_system_scheduler_init();
	
	rt_thread_init(&rt_flag1_thread,
								"flag1",
								flag1_thread_entry,
								RT_NULL,
								&rt_flag1_thread_stack[0],
								sizeof(rt_flag1_thread_stack));
	
	rt_list_insert_before( &(rt_thread_priority_table[0]), &(rt_flag1_thread.tlist));
	
	rt_thread_init(&rt_flag2_thread,
								"flag2",
								flag2_thread_entry,
								RT_NULL,
								&rt_flag2_thread_stack[0],
								sizeof(rt_flag2_thread_stack));
	
	rt_list_insert_before( &(rt_thread_priority_table[1]), &(rt_flag2_thread.tlist));

	rt_system_scheduler_start();
}









