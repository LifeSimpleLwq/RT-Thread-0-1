#include <rtthread.h>
#include <rthw.h>
#include "ARMCM3.h"

extern	rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];

rt_uint8_t flag1;
rt_uint8_t flag2;
rt_uint8_t flag3;

// 定义线程控制块
struct rt_thread rt_flag1_thread;
struct rt_thread rt_flag2_thread;
struct rt_thread rt_flag3_thread;

// 4字节对齐
ALIGN(RT_ALIGN_SIZE)
// 定义两个线程栈
rt_uint8_t rt_flag1_thread_stack[512];
rt_uint8_t rt_flag2_thread_stack[512];
rt_uint8_t rt_flag3_thread_stack[512];

void flag1_thread_entry(void *p_arg);
void flag2_thread_entry(void *p_arg);
void flag3_thread_entry(void *p_arg);

int main(void)
{
	rt_hw_interrupt_disable();
	
	SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);
	
	rt_system_timer_init();
	
	/* 调度器初始化 */ 
	rt_system_scheduler_init();
	
	/* 空闲函数初始化 */
	rt_thread_idle_init();
	
	/* 初始化线程 */ 
	rt_thread_init(&rt_flag1_thread,			/* 线程控制块 */ 
						"flag1_t",					/* 线程名字，字符串形式 */ 
						flag1_thread_entry,		/* 线程入口 */ 
						RT_NULL,						/* 线程入口形参 */ 
						&rt_flag1_thread_stack,			/* 线程栈起始地址 */ 
						sizeof(rt_flag1_thread_stack),/* 线程栈大小 */ 
						2);	
	/* 将线程插入到就绪列表 */ 
	//rt_list_insert_before(&(rt_thread_priority_table[0]),&(rt_flag1_thread.tlist));	
	rt_thread_startup(&rt_flag1_thread);
	
	rt_thread_init(&rt_flag2_thread,			/* 线程控制块 */ 
						"flag2_t",					/* 线程名字，字符串形式 */ 
						flag2_thread_entry,		/* 线程入口 */ 
						RT_NULL,						/* 线程入口形参 */ 
						&rt_flag2_thread_stack,		/* 线程栈起始地址 */ 
						sizeof(rt_flag2_thread_stack),/* 线程栈大小 */ 
						3);	
	/* 将线程插入到就绪列表 */ 					
	//rt_list_insert_before(&(rt_thread_priority_table[1]),&(rt_flag2_thread.tlist));
	rt_thread_startup(&rt_flag2_thread);
	
	rt_thread_init(&rt_flag3_thread,
						"flag3_t",
						flag3_thread_entry,
						RT_NULL,
						&rt_flag3_thread_stack,
						sizeof(rt_flag3_thread_stack),
						4);
	rt_thread_startup(&rt_flag3_thread);
						
	rt_system_scheduler_start();
}

/*
*************************************************************************
*                               函数实现
*************************************************************************
*/
/* 软件延时 */
void delay(rt_uint32_t count)
{
	for(;count!=0;count--);
}

/* 线程1 */ 
void flag1_thread_entry(void *p_arg)
{
	while(1)
	{
#if 0 
		flag1 = 1;
		delay(100);
		flag1 = 0;
		delay(100);
		/* 线程切换，这里是手动切换 */		
		rt_schedule();
#else 		
		flag1 = 1;
		rt_thread_delay(2);
		flag1 = 0;
		rt_thread_delay(2);
#endif
	}
}	
/* 线程2 */ 
void flag2_thread_entry(void *p_arg)
{
	while(1)
	{
#if 0
		flag2 = 1;
		delay(100);
		flag2 = 0;
		delay(100);
		
		/* 线程切换，这里是手动切换 */		
		rt_schedule();
#else 
		flag2 = 1;
		rt_thread_delay(20);
		flag2 = 0;
		rt_thread_delay(20);
#endif
	}
}

void flag3_thread_entry(void *p_arg)
{
	while(1)
	{
		flag3 = 1;
		rt_thread_delay(RT_TICK_PER_SECOND);
		flag3 = 0;
		rt_thread_delay(RT_TICK_PER_SECOND);
	}
}

void SysTick_Handler(void)
{
	rt_interrupt_enter();
	
	rt_tick_increase();
	
	rt_interrupt_leave();
}
















