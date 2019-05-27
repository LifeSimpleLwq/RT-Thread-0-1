#include "rtthread.h"


static struct rt_object_information
rt_object_container[RT_Object_Info_Unknown] = {
	// 线程
	{
		RT_Object_Class_Thread,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_Thread),
		sizeof(struct rt_thread)
	},
	
#ifdef RT_USING_SEMAPHORE
	// 信号量
	{
		RT_Object_Class_Semaphore,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_Semaphore),
		sizeof(struct rt_semaphore)
	}
#endif	
	
#ifdef RT_USING_MUTEX
	// 互斥量
	{
		RT_Object_Class_Mutex,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_Mutex),
		sizeof(struct rt_mutex)
	}
#endif	
	
#ifdef RT_USING_EVENT
	// 事件
	{
		RT_Object_Class_Event,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_Event),
		sizeof(struct rt_event)
	}
#endif		
	
#ifdef RT_USING_MAILBOX
	// 邮箱
	{
		RT_Object_Class_MailBox,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_MailBox),
		sizeof(struct rt_mailbox)
	}
#endif	
	
#ifdef RT_USING_MASSAGEQUEUE
	// 消息队列
	{
		RT_Object_Class_MassgaQueue,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_MassageQueue),
		sizeof(struct rt_massagequeue)
	}
#endif		
	
#ifdef RT_USING_MEMHEAP
	// 内存堆
	{
		RT_Object_Class_MemHeap,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_MemHeap),
		sizeof(struct rt_memheap)
	}
#endif		
	
#ifdef RT_USING_MEMPOOL
	// 内存池
	{
		RT_Object_Class_MemPool,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_MemPool),
		sizeof(struct rt_mempool)
	}
#endif		
	
#ifdef RT_USING_DEVICE
	// 设备
	{
		RT_Object_Class_Device,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_Device),
		sizeof(struct rt_device)
	}
#endif		
	
#ifdef RT_USING_MODULE
	// 模块
	{
		RT_Object_Class_Module,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_Module),
		sizeof(struct rt_module)
	}
#endif	
}

