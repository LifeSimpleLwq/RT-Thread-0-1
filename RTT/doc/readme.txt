rtthread
	bsp					存放板级支持包
	components/finsh	存放RTT组件，
	include				存放头文件
	src					存放内核源码
	

preblom:
1，rt_hw_stack_init初始化，为何堆栈要-4
2，堆栈为何要初始化为0xdeadbeef
3，PendSV优先级
4, 什么情况下执行_reswitch
5，thread->number_mask = 0;  为啥初始化为0
	在rt_thread_startup中对number_mask进行了初始化
6,timerout_tick 要求不能大于 RT_TICK_MAX /2？
7, 保存中断的变量为何要用register修饰

/*  */

/**  
 *
 */







