#ifndef __RT_DEF_H__
#define __RT_DEF_H__

#include <rtconfig.h>

typedef signed char 		rt_int8_t;
typedef signed short		rt_int16_t;
typedef signed long		rt_int32_t;
typedef unsigned char 	rt_uint8_t;
typedef unsigned short  rt_uint16_t;
typedef unsigned long 	rt_uint32_t;
typedef int					rt_bool_t;			

typedef long					rt_base_t;
typedef unsigned long		rt_ubase_t;

typedef rt_base_t				rt_err_t;
typedef rt_uint32_t			rt_time_t;
typedef rt_uint32_t			rt_tick_t;
typedef rt_base_t				rt_flag_t;
typedef	rt_ubase_t			rt_size_t;
typedef rt_ubase_t			rt_dev_t;
typedef	rt_base_t			rt_off_t;

// bool type
#define 		RT_TRUE			1
#define		RT_FALSE			0

#ifdef		__CC_ARM	
#define		rt_inline			static __inline
#define		ALIGN(n)			__attribute__((aligned(n)))

#elif			defined(__IAR_SYSTEMS_ICC__)
#define 		rt_inline			static __inline
#define		ALIGN(n)			PRAGMA(data_alignment=n)

#elif			defined(__GNUC__)
#define		rt_inline			static __inline
#define		ALIGN(n)			__attribute__((aligned(n)))
#else 
#error not supported tool chain
#endif

#define		RT_ALIGN(size,align)				(((size) + (align) -1) & ~((align) - 1))
#define		RT_ALIGN_DOWN(size,align)		((size) & ~ ((align) -1))

#define		RT_NULL				(0)

#define		RT_EOK				0
#define		RT_ERROR				1
#define		RT_ETIMEOUT			2
#define		RT_EFULL				3
#define		RT_EEMPTY			4
#define		RT_ENOMEM			5
#define		RT_ENOSYS			6
#define		RT_EBUSY				7
#define		RT_EIO				8
#define		RT_EINTR				9
#define		RT_EINVAL			10

#define 		RT_THREAD_INIT			0X00
#define 		RT_THREAD_READY		0X01
#define 		RT_THREAD_SUSPEND		0X02
#define 		RT_THREAD_RUNNING		0X03
#define 		RT_THREAD_BLOCK		RT_THREAD_SUSPEND
#define 		RT_THREAD_CLOSE		0X04
#define 		RT_THREAD_STAT_MASK	0X0F

#define 		RT_THREAD_STAT_SIGNAL			0X10
#define 		RT_THREAD_STAT_SIGNAL_READY	(RT_THREAD_STAT_SIGNAL | \
														 RT_THREAD_READY)
#define 		RT_THREAD_STAT_SIGNAL_SUBSPEND	0X20
#define 		RT_THREAD_STAT_SIGNAL_MASK			0XF0

#define		RT_TIMER_FLAG_DEACTIVATED	0X0
#define		RT_TIMER_FLAG_ACTIVATED		0X1
#define		RT_TIMER_FLAG_ONE_SHOT		0X0
#define		RT_TIMER_FLAG_PERIODIC		0X2

#define		RT_TIMER_FLAG_HARD_TIMER	0X0

#define		RT_TIMER_FLAG_SOFT_TIMER	0X4

#define 	RT_TIMER_CTRL_GET_TIME		0X1
#define	RT_TIMER_CTRL_SET_TIME		0X0
#define	RT_TIMER_CTRL_SET_ONESHOT	0X2
#define	RT_TIMER_CTRL_SET_PERIODIC	0X3

#ifndef RT_TIMER_SKIP_LIST_LEVEL
#define RT_TIMER_SKIP_LIST_LEVEL          1
#endif

struct rt_timer
{
	struct rt_object parent;
	
	rt_list_t row[RT_TIMER_SKIP_LIST_LEVEL];
	
	void (*timeout_func)(void *parameter);
	void 								*parameter;
	
	rt_tick_t	init_tick;
	rt_tick_t	timeout_tick;
};
typedef struct rt_timer *rt_timer_t;


struct rt_list_node
{
		struct	rt_list_node *next;
		struct  rt_list_node *prev;
};
typedef struct rt_list_node rt_list_t;

struct rt_thread
{		
		char			name[RT_NAME_MAX];
		rt_uint8_t  type;
		rt_uint8_t	flag;
		rt_list_t 	list;
		rt_list_t	tlist;
	
		void 			*sp;
		void			*entry;
		void  		*parameter;
		void			*stack_addr;
		rt_uint32_t static_size;
		rt_ubase_t	remaining_tick;
		
		rt_uint8_t	current_priority;
		rt_uint8_t 	init_priority;
		rt_uint32_t	number_mask;
	
		rt_err_t		error;
		rt_uint8_t	stat;		/* 线程状态 */
	
		struct rt_timer thread_timer;	/* 内置线程的定时器 */
};
typedef struct rt_thread *rt_thread_t;

enum rt_object_class_type
{
	RT_Object_Class_Thread = 0,
	RT_Object_Class_Semaphore,
	RT_Object_Class_Mutex,
	RT_Object_Class_Event,
	RT_Object_Class_MailBox,
	RT_Object_Class_MessageQueue,
	RT_Object_Class_MemHeap,
	RT_Object_Class_MemPool,
	RT_Object_Class_Device,
	RT_Object_Class_Timer,
	RT_Object_Class_Module,
	RT_Object_Class_Unknown,
	RT_Object_Class_Static = 0x80
};

struct rt_object_information
{
	enum rt_object_class_type type;
	rt_list_t		object_list;
	rt_size_t		object_size;
};

struct rt_object
{
	char	name[RT_NAME_MAX];
	rt_uint8_t type;
	rt_uint8_t flag;
	
	rt_list_t list;
};
typedef struct rt_object *rt_object_t;
		


#endif
