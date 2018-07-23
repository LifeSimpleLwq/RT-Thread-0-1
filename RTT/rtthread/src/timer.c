#include <rtthread.h>
#include <rthw.h>

static rt_list_t rt_timer_list[RT_TIMER_SKIP_LIST_LEVEL];

void rt_system_timer_init(void)
{
	int i;
	
	for(i = 0; i < sizeof(rt_timer_list); i++)
	{
		rt_list_init(rt_timer_list +i);
	}
}

static void _rt_timer_init(rt_timer_t timer,
									void (*timeout)(void *parameter),
									void *parameter,
									rt_tick_t  time,
									rt_uint8_t flag)
{
	int i;
	
	timer->parent.flag = flag;
	
	/* 先设置为非激活状态 */ 
	timer->parent.flag = ~RT_TIMER_FLAG_ACTIVATED;
	
	timer->timeout_func = timeout;
	timer->parameter    = parameter;
	
	timer->timeout_tick = 0;
	/* 初始化定时器需要的超时节拍数 */
	timer->init_tick = time;
	
	for (i = 0; i < RT_TIMER_SKIP_LIST_LEVEL; i++)
	{
		rt_list_init(&(timer->row[i]));
	}
}

void rt_timer_init(rt_timer_t timer,
						 const char *name,
						 void (*timeout)(void *parameter),
						 void *parameter,
						 rt_tick_t  time,
						 rt_uint8_t flag)
{
	rt_object_init((rt_object_t)timer, RT_Object_Class_Timer,name);
	
	_rt_timer_init(timer, timeout, parameter, time, flag);
}

rt_inline void _rt_timer_remove(rt_timer_t timer)
{
	int i;
	
	for (i = 0; i < RT_TIMER_SKIP_LIST_LEVEL; i++)
	{
		rt_list_remove(&timer->row[i]);
	}
}

rt_err_t rt_timer_stop(rt_timer_t timer)
{
	register rt_base_t level;
	
	if (!(timer->parent.flag & RT_TIMER_FLAG_ACTIVATED))
		return -RT_ERROR;
	
	level = rt_hw_interrupt_disable();
	
	_rt_timer_remove(timer);
	
	rt_hw_interrupt_enable(level);

	timer->parent.flag &= ~RT_TIMER_FLAG_ACTIVATED;
	
	return RT_EOK;
}

rt_err_t rt_timer_control(rt_timer_t timer, int cmd, void *arg)
{
	switch (cmd)
	{
		case RT_TIMER_CTRL_GET_TIME:
		
			break;
		case RT_TIMER_CTRL_SET_TIME:
			
			break;
		case RT_TIMER_CTRL_SET_ONESHOT:
			
			break;
		case RT_TIMER_CTRL_SET_PERIODIC:
			
			break;
	}
	
	return RT_EOK;
}

rt_err_t rt_timer_start(rt_timer_t timer)
{
	unsigned int row_lvl = 0;
	rt_list_t *timer_list;
	register	rt_base_t level;
	rt_list_t *row_head[RT_TIMER_SKIP_LIST_LEVEL];
	unsigned int tst_nr;
	static unsigned int random_nr;
	
	level = rt_hw_interrupt_disable();
	
	_rt_timer_remove(timer);
	
	timer->parent.flag &= ~RT_TIMER_FLAG_ACTIVATED;
	
	rt_hw_interrupt_enable(level);

	timer->timeout_tick = rt_tick_get() +timer->init_tick;
	
	level = rt_hw_interrupt_disable();
	
	row_head[0] = &timer_list[0];
	
	
	rt_hw_interrupt_enable(level);
}




