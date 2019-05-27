#ifndef __RT_DEF_H__
#define __RT_DEF_H__

#include "rtconfig.h"

/* RT-Thread �������������ض��� */
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


/* �������������ض��� */
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


/**		�����붨��	**/
#define		RT_EOK						0			// û�д���
#define		RT_ERROR					1			// һ���������
#define		RT_ETIMEOUT				2			// ��ʱ
#define		RT_EFULL					3			// ��Դ����
#define		RT_EEMPTY					4			// ��ԴΪ��
#define		RT_ENOMEM					5			// û���ڴ�
#define		RT_ENOSYS					6			// no system
#define		RT_EBUSY					7			// æ
#define		RT_EIO						8			// IO����
#define		RT_EINTR					9			// �ж�ϵͳ����
#define		RT_EINVAL					10		// ��Ч�β�

/** �߳�״̬���� **/	
#define RT_THREAD_INIT								0x00									// ��ʼ̬
#define RT_THREAD_READY								0x01									// ����̬
#define RT_THREAD_SUSPEND							0x02									// ����̬
#define RT_THREAD_RUNNING							0x03									// ����̬
#define RT_THREAD_BLOCK								RT_THREAD_SUSPEND			// ����̬
#define	RT_THREAD_CLOSE								0x04									// �ر�̬
#define RT_THREAD_STAT_MASK						0x0f	

#define RT_THREAD_STAT_SIGNAL					0x10
#define RT_THREAD_STAT_SIGNAL_READY		(RT_THREAD_STAT_SIGNAL | \
																			RT_THREAD_READY)
#define RT_THREAD_STAT_SIGNAL_SUSPEND	0x20
#define RT_THREAD_STAT_SIGNAL_MASK		0xf0

// ˫������ڵ���������
struct rt_list_node
{
	struct rt_list_node *next;	// ָ���һ���ڵ�
	struct rt_list_node *prev;	// ָ��ǰһ���ڵ�
};
typedef struct rt_list_node rt_list_t;

// ��������
enum rt_object_class_type
{
	RT_Object_Class_Thread = 0,			// �߳�
	RT_Object_Class_Semaphore,			// �ź���
	RT_Object_Class_Mutex,					// ������
	RT_Object_Class_Event,					// �¼�
	RT_Object_Class_MailBox,				// ����
	RT_Object_Class_MessageQueue,		// ��Ϣ����
	RT_Object_Class_MemHeap,				// �ڴ��
	RT_Object_Class_MemPool,				// �ڴ��
	RT_Object_Class_Device,					// �豸����
	RT_Object_Class_Timer,					// ��ʱ��
	RT_Object_Class_Module,					// ģ��
	RT_Object_Class_Unknow,					// δ֪
	RT_Object_Class_Static = 0x80		// ��̬
};

// ������ƿ�
struct rt_object
{
	char name[RT_NAME_MAX];		// ��������
	rt_uint8_t type;					// ����
	rt_uint8_t flag;					// ״̬
	
	rt_list_t list;						
};
typedef struct rt_object *rt_object_t;


// �߳̿��ƿ�
struct rt_thread
{
	/* rt���� */
	char 				name[RT_NAME_MAX];
	rt_uint8_t	type;
	rt_uint8_t	flag;
	rt_list_t 	list;
	
	rt_list_t		tlist;						// �߳�����ڵ�
	
	void 				*sp;							// �߳�ջָ��
	void 				*entry;						// �߳���ڵ�ַ
	void 				*parameter;				// �߳��β�
	void 				*stack_addr;			// �߳�ջ��ʼ��ַ
	
	rt_uint32_t stack_size;				// �߳�ջ��С����λΪ�ֽ�
	
	rt_ubase_t 	remaining_tick; 	// ����ʵ��������ʱ
	
	rt_uint8_t 	current_priority;	// ��ǰ���ȼ�
	rt_uint8_t 	init_priority;		// ��ʼ�����ȼ�
	rt_uint32_t number_mask;			// ��ǰ���ȼ�����
	
	rt_err_t 		error;						// ������
	
	rt_uint8_t 	stat;							// �߳�״̬
	
};
typedef struct rt_thread *rt_thread_t;

struct rt_object_information
{
	enum rt_object_class_type type;
	rt_list_t				object_list;
	rt_size_t				object_size;
};


#endif /*		__RT_DEF_H__	*/














