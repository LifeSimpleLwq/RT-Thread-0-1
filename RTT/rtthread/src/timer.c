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
	
	/*  */
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

/**
  * 该函数将停止一个定时器
  */
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

/**
  * 该函数将获取或者设置一个定时器的一些选项
  * @param timer 将要被设置或者获取的定时器
  * @param cmd   控制命令
  * @param arg   形参
  */
rt_err_t rt_timer_control(rt_timer_t timer, int cmd, void *arg)
{
	switch (cmd)
	{
		case RT_TIMER_CTRL_GET_TIME:
			*(rt_tick_t *)arg = timer->init_tick;
			break;
		case RT_TIMER_CTRL_SET_TIME:
			timer->init_tick = *(rt_tick_t *)arg;
			break;
		case RT_TIMER_CTRL_SET_ONESHOT:
			timer->parent.flag &= ~RT_TIMER_FLAG_PERIODIC;
			break;
		case RT_TIMER_CTRL_SET_PERIODIC:
			timer->parent.flag |= RT_TIMER_FLAG_PERIODIC;
			break;
	}
	
	return RT_EOK;
}

/**  
 *	启动定时器
 */
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
	
	/* 获取 timeout_tick,
	   最大的timeout_tick 不能大于 RT_TICK_MAX/2 */
	timer->timeout_tick = rt_tick_get() +timer->init_tick;
	
	level = rt_hw_interrupt_disable();
	
	/* 将定时器插入到定时器列表 */
	/* 获取系统定时器列表根节点地址,rt_timer_list是一个全局变量 */
	timer_list = rt_timer_list;
	
	row_head[0] = &timer_list[0];
	
	/* 因为 RT_TIMER_SKIP_LIST_LEVEL = 1，该循环只会执行一次 */
	for (row_lvl = 0; row_lvl < RT_TIMER_SKIP_LIST_LEVEL; row_lvl++)
	{
		/* 当系统定时器列表 timer_list 为空时，该循环不执行 */
		for (;row_head[row_lvl] != timer_list[row_lvl].prev; 
				row_head[row_lvl] = row_head[row_lvl]->next)
		{
			struct rt_timer *t;
			
			rt_list_t *p = row_head[row_lvl]->next;
			
			t = rt_list_entry(p,
							  struct rt_timer,
							  row[row_lvl]);
			
			/* 两个定时器超时时间相同，则继续在定时器列表中寻找下一个节点 */
			if ((t->timeout_tick - timer->timeout_tick) == 0)
			{
				continue;
			}
			else if ((t->timeout_tick - timer->timeout_tick) < RT_TICK_MAX/2) 
			{
				break;
			}
		}
		
		/* 条件不会为真，不会被执行 */
		if (row_lvl != RT_TIMER_SKIP_LIST_LEVEL -1)
		{
			row_head[row_lvl + 1] = row_head[row_lvl] + 1;
		}
	}
	
	/* random_nr是一个静态变量，用于记录启动了多少个定时器 */
	random_nr++;
	tst_nr = random_nr;
	
	/* 将定时器插入到定时器列表 */
	rt_list_insert_after(row_head[RT_TIMER_SKIP_LIST_LEVEL - 1],
						&(timer->row[RT_TIMER_SKIP_LIST_LEVEL - 1]));
	
	/* RT_TIMER_SKIP_LIST_LEVEL = 1，该for循环永远不会执行 */
	for (row_lvl = 2;row_lvl <= RT_TIMER_SKIP_LIST_LEVEL; row_lvl++)
	{
		if (!(tst_nr & RT_TIMER_SKIP_LIST_MASK))
			rt_list_insert_after(row_head[RT_TIMER_SKIP_LIST_LEVEL - row_lvl],
								&(timer->row[RT_TIMER_SKIP_LIST_LEVEL - row_lvl]));
		else 
			break;
		
		tst_nr >>= (RT_TIMER_SKIP_LIST_MASK +1) >> 1;
	}
	
	/* 设置定时器标志位为激活态 */
	timer->parent.flag |= RT_TIMER_FLAG_ACTIVATED;
	
	rt_hw_interrupt_enable(level);
	
	return -RT_EOK;
}

/**  
 * 该函数用于扫描系统定时器列表，当有超时事件发生时，
 * 就调用对应的超时函数
 *
 * @note 该函数在操作系统定时器中断中被调用
 */
void rt_timer_check(void)
{
	struct rt_timer *t;
	rt_tick_t current_tick;
	register rt_base_t level;
	
	/* 获取系统时基 rt_tick的值 */
	current_tick = rt_tick_get();
	
	level = rt_hw_interrupt_disable();
	
	/* 系统定时器列表不为空，则扫描定时器列表 */
	while (!rt_list_isempty(&rt_timer_list[RT_TIMER_SKIP_LIST_LEVEL - 1]))
	{
		/* 获取第一个节点定时器的地址 */
		t = rt_list_entry(rt_timer_list[RT_TIMER_SKIP_LIST_LEVEL -1].next,
						 struct rt_timer,
						 row[RT_TIMER_SKIP_LIST_LEVEL - 1]);
		
		if ((current_tick - t->timerout_tick) < RT_TICK_MAX/2)
		{
			/* 先将定时器从系统定时器列表移除 */
			_rt_timer_remove(t);
			
			t->timerout_func(t->parameter);
			
			current_tick = rt_tick_get();
			
			/* 周期定时器 */
			if ((t->parent.flag & RT_TIMER_FLAG_PERIODIC) &&
				(t->parent.flag & RT_TIMER_FLAG_ACTIVATED))
			{
				t->parent.flag &= ~RT_TIMER_FLAG_ACTIVATED;
				rt_timer_start(t);
			}
			/* 单次定时器 */
			else 
			{
				t->parent.flag &= ~RT_TIMER_FLAG_ACTIVATED;
			}
		}
		else 
			break;
	}
	rt_hw_interrupt_enable(level);
}


