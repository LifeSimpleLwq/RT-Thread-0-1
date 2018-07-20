#include <rtthread.h>
#include <rthw.h>

enum rt_object_info_type
{
	RT_Object_Info_Thread = 0,
#ifdef	RT_USING_SEMAPHORE	
	RT_Object_Info_Semaphore,
#endif
#ifdef	RT_USING_MUTEX		
	RT_Object_Info_Mutex,
#endif
#ifdef	RT_USING_EVENT	
	RT_Object_Info_Event,
#endif	
#ifdef	RT_USING_MAILBOX			
	RT_Object_Info_MailBox,
#endif
#ifdef	RT_USING_MESSAGEQUEUE		
	RT_Object_Info_MessageQueue,
#endif
#ifdef	RT_USING_MEMHEAP		
	RT_Object_Info_MemHeap,
#endif
#ifdef	RT_USING_MEMPOOL		
	RT_Object_Info_MemPool,
#endif	
#ifdef	RT_USING_DEVICE		
	RT_Object_Info_Device,
#endif

	RT_Object_Info_Timer,

#ifdef	RT_USING_MODULE	
	RT_Object_Info_Module,
#endif		
	RT_Object_Info_Unknown,
};


#define	_OBJ_CONTAINER_LIST_INIT(c)	\
{&(rt_object_container[c].object_list),&(rt_object_container[c].object_list)}

static struct rt_object_information rt_object_container[RT_Object_Info_Unknown] = 
{
	{
		RT_Object_Class_Thread,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_Thread),
		sizeof(struct rt_thread)
	},
#ifdef	RT_USING_SEMAPHORE
	{
		RT_Object_Class_Semaphore,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_Semaphore),
		sizeof(struct rt_semaphore)
	},
#endif

#ifdef	RT_USING_MUTEX
	{
		RT_Object_Class_Mutex,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_Mutex),
		sizeof(struct rt_mutex)
	},
#endif	

#ifdef	RT_USING_EVENT
	{
		RT_Object_Class_Event,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_Event),
		sizeof(struct rt_event)
	},
#endif
	
#ifdef	RT_USING_MAILBOX
	{
		RT_Object_Class_MailBox,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_MailBox),
		sizeof(struct rt_mailbox)
	},
#endif	

#ifdef	RT_USING_MESSAGEQUEUE
	{
		RT_Object_Class_MessageQueue,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_MessageQueue),
		sizeof(struct rt_messagequeue)
	},
#endif	

#ifdef	RT_USING_MEMHEAP
	{
		RT_Object_Class_MemHeap,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_MemHeap),
		sizeof(struct rt_menheap)
	},
#endif

#ifdef	RT_USING_MEMPOOL
	{
		RT_Object_Class_MemPool,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_MemPool),
		sizeof(struct rt_menpool)
	},
#endif

#ifdef	RT_USING_DEVICE
	{
		RT_Object_Class_Device,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_Device),
		sizeof(struct rt_device)
	},
#endif

//	{
//		RT_Object_Class_Timer,
//		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_Timer),
//		sizeof(struct rt_timer)
//	},

#ifdef	RT_USING_MODULE
	{
		RT_Object_Class_Module,
		_OBJ_CONTAINER_LIST_INIT(RT_Object_Info_Module),
		sizeof(struct rt_Module)
	},
#endif
};

struct rt_object_information *
rt_object_get_information(enum rt_object_class_type type)
{
	int index;
	
	for(index = 0; index < RT_Object_Class_Unknown; index++)
	{
		if(rt_object_container[index].type == type)
			return &rt_object_container[index];
	}
	
	return RT_NULL;
}

// 初始化对象，并将对象添加到对象容器中
void rt_object_init(struct rt_object 				*object,
						  enum rt_object_class_type 	type,
						  const char 						*name)
{
	register rt_base_t temp;
	struct rt_object_information *information;
	
	information = rt_object_get_information(type);
	
	object->type = type | RT_Object_Class_Static;
	
	rt_strncpy(object->name,name,RT_NAME_MAX);
	
	temp = rt_hw_interrupt_disable();
	
	rt_list_insert_after(&(information->object_list),&(object->list));
	
	rt_hw_interrupt_enable(temp);
}











