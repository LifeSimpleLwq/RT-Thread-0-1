#ifndef __RT_DEF_H__
#define __RT_DEF_H__

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
#define		RT_EOK					0
#define		RT_ERROR				1
#define		RT_ETIMEOUT			2
#define		RT_EFULL				3
#define		RT_EEMPTY				4
#define		RT_ENOMEM				5
#define		RT_ENOSYS				6
#define		RT_EBUSY				7
#define		RT_EIO					8
#define		RT_EINTR				9
#define		RT_EINVAL				10


// 双向链表节点数据类型
struct rt_list_node
{
	struct rt_list_node *next;	// 指向后一个节点
	struct rt_list_node *prev;	// 指向前一个节点
};
typedef struct rt_list_node rt_list_t;

// 线程控制块
struct rt_thread
{
	void 				*sp;					// 线程栈指针
	void 				*entry;				// 线程入口地址
	void 				*parameter;		// 线程形参
	void 				*stack_addr;	// 线程栈起始地址
	
	rt_uint32_t stack_size;		// 线程栈大小，单位为字节
		
	rt_list_t		tlist;				// 线程链表节点
	
};
typedef struct rt_thread *rt_thread_t;



#endif /*		__RT_DEF_H__	*/














