#ifndef __RT_DEF_H__
#define __RT_DEF_H__

#include "rtconfig.h"

/* RT-Thread 基础数据类型重定义 */
typedef signed char 			rt_int8_t;
typedef signed short 			rt_int16_t;
typedef signed int 				rt_int32_t;
typedef unsigned char 		rt_uint8_t;
typedef unsigned short 		rt_uint16_t;
typedef unsigned int 			rt_uint32_t;
typedef int 							rt_bool_t;

/* 32bit CPU */
typedef long 							rt_base_t;
typedef unsigned long 		rt_ubase_t;

typedef rt_base_t					rt_err_t;
typedef rt_uint32_t				rt_time_t;
typedef rt_uint32_t				rt_tick_t;
typedef rt_base_t					rt_flag_t;
typedef rt_ubase_t				rt_size_t;
typedef	rt_ubase_t				rt_dev_t;
typedef	rt_base_t 				rt_off_t;


/* 布尔数据类型重定义 */
#define RT_TRUE						1
#define RT_FALSH					0


#ifdef __CC_ARM
#define rt_inline 				static __inline
#define ALIGN(n)					__attribute__((aligned(n)))

#elif	defined (__IAR_SYSTEMS_ICC__)
#define rt_inline					static inline
#define ALIGN(n)					PPAGMA(data_alignment=n)

#elif defined (__GUNC__)
#define rt_inline					static __inline
#define ALIGN(n)					__attribute__((aligned(n)))
#else 
#error not supported tool chain
#endif 

#define RT_ALIGN(size, align)				(((size) + (align) - 1) & ~((align) - 1))
#define RT_ALIGN_DOWN(size, align)	((size) & ~((align) - 1))

#define RT_NULL						(0)


/**		错误码定义	**/
#define		RT_EOK						0			// 没有错误
#define		RT_ERROR					1			// 一个常规错误
#define		RT_ETIMEOUT				2			// 超时
#define		RT_EFULL					3			// 资源已满
#define		RT_EEMPTY					4			// 资源为空
#define		RT_ENOMEM					5			// 没有内存
#define		RT_ENOSYS					6			// no system
#define		RT_EBUSY					7			// 忙
#define		RT_EIO						8			// IO错误
#define		RT_EINTR					9			// 中断系统调用
#define		RT_EINVAL					10		// 无效形参

/** 线程状态定义 **/	
#define RT_THREAD_INIT								0x00									// 初始态
#define RT_THREAD_READY								0x01									// 就绪态
#define RT_THREAD_SUSPEND							0x02									// 挂起态
#define RT_THREAD_RUNNING							0x03									// 运行态
#define RT_THREAD_BLOCK								RT_THREAD_SUSPEND			// 阻塞态
#define	RT_THREAD_CLOSE								0x04									// 关闭态
#define RT_THREAD_STAT_MASK						0x0f	

#define RT_THREAD_STAT_SIGNAL					0x10
#define RT_THREAD_STAT_SIGNAL_READY		(RT_THREAD_STAT_SIGNAL | \
																			RT_THREAD_READY)
#define RT_THREAD_STAT_SIGNAL_SUSPEND	0x20
#define RT_THREAD_STAT_SIGNAL_MASK		0xf0

// 双向链表节点数据类型
struct rt_list_node
{
	struct rt_list_node *next;	// 指向后一个节点
	struct rt_list_node *prev;	// 指向前一个节点
};
typedef struct rt_list_node rt_list_t;

// 对象类型
enum rt_object_class_type
{
	RT_Object_Class_Thread = 0,			// 线程
	RT_Object_Class_Semaphore,			// 信号量
	RT_Object_Class_Mutex,					// 互斥量
	RT_Object_Class_Event,					// 事件
	RT_Object_Class_MailBox,				// 邮箱
	RT_Object_Class_MessageQueue,		// 消息队列
	RT_Object_Class_MemHeap,				// 内存堆
	RT_Object_Class_MemPool,				// 内存池
	RT_Object_Class_Device,					// 设备驱动
	RT_Object_Class_Timer,					// 定时器
	RT_Object_Class_Module,					// 模块
	RT_Object_Class_Unknow,					// 未知
	RT_Object_Class_Static = 0x80		// 静态
};

// 对象控制块
struct rt_object
{
	char name[RT_NAME_MAX];		// 对象名称
	rt_uint8_t type;					// 类型
	rt_uint8_t flag;					// 状态
	
	rt_list_t list;						
};
typedef struct rt_object *rt_object_t;


// 线程控制块
struct rt_thread
{
	/* rt对象 */
	char 				name[RT_NAME_MAX];
	rt_uint8_t	type;
	rt_uint8_t	flag;
	rt_list_t 	list;
	
	rt_list_t		tlist;						// 线程链表节点
	
	void 				*sp;							// 线程栈指针
	void 				*entry;						// 线程入口地址
	void 				*parameter;				// 线程形参
	void 				*stack_addr;			// 线程栈起始地址
	
	rt_uint32_t stack_size;				// 线程栈大小，单位为字节
	
	rt_ubase_t 	remaining_tick; 	// 用于实现阻塞延时
	
	rt_uint8_t 	current_priority;	// 当前优先级
	rt_uint8_t 	init_priority;		// 初始化优先级
	rt_uint32_t number_mask;			// 当前优先级掩码
	
	rt_err_t 		error;						// 错误码
	
	rt_uint8_t 	stat;							// 线程状态
	
};
typedef struct rt_thread *rt_thread_t;

struct rt_object_information
{
	enum rt_object_class_type type;
	rt_list_t				object_list;
	rt_size_t				object_size;
};


#endif /*		__RT_DEF_H__	*/














