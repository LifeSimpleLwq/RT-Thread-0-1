#ifndef __RT_DEF_H__
#define __RT_DEF_H__

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

struct	rt_list_node
{
		struct	rt_list_node *next;
		struct  rt_list_node *prev;
};
typedef struct rt_list_node rt_list_t;

struct	rt_thread
{
		void 	*sp;
		void	*entry;
		void  *parameter;
		void	*stack_addr;
		rt_uint32_t 	static_size;
	
		rt_list_t		tlist;
};
typedef struct rt_thread *rt_thread_t;



#endif
