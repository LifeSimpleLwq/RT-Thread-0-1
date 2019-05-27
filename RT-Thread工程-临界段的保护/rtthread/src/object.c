#include "rtthread.h"


static struct rt_object_information
rt_object_container[RT_Object_Info_Unknown] = {
	// �߳�
	{
		RT_Object_Class_Thread,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_Thread),
		sizeof(struct rt_thread)
	},
	
#ifdef RT_USING_SEMAPHORE
	// �ź���
	{
		RT_Object_Class_Semaphore,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_Semaphore),
		sizeof(struct rt_semaphore)
	}
#endif	
	
#ifdef RT_USING_MUTEX
	// ������
	{
		RT_Object_Class_Mutex,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_Mutex),
		sizeof(struct rt_mutex)
	}
#endif	
	
#ifdef RT_USING_EVENT
	// �¼�
	{
		RT_Object_Class_Event,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_Event),
		sizeof(struct rt_event)
	}
#endif		
	
#ifdef RT_USING_MAILBOX
	// ����
	{
		RT_Object_Class_MailBox,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_MailBox),
		sizeof(struct rt_mailbox)
	}
#endif	
	
#ifdef RT_USING_MASSAGEQUEUE
	// ��Ϣ����
	{
		RT_Object_Class_MassgaQueue,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_MassageQueue),
		sizeof(struct rt_massagequeue)
	}
#endif		
	
#ifdef RT_USING_MEMHEAP
	// �ڴ��
	{
		RT_Object_Class_MemHeap,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_MemHeap),
		sizeof(struct rt_memheap)
	}
#endif		
	
#ifdef RT_USING_MEMPOOL
	// �ڴ��
	{
		RT_Object_Class_MemPool,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_MemPool),
		sizeof(struct rt_mempool)
	}
#endif		
	
#ifdef RT_USING_DEVICE
	// �豸
	{
		RT_Object_Class_Device,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_Device),
		sizeof(struct rt_device)
	}
#endif		
	
#ifdef RT_USING_MODULE
	// ģ��
	{
		RT_Object_Class_Module,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_Module),
		sizeof(struct rt_module)
	}
#endif	
}

