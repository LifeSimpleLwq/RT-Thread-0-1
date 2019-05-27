#include "rtthread.h"
#include "rthw.h"

// �߳����ȼ���
rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];

// �߳̾������ȼ���
rt_uint32_t rt_thread_ready_priority_group;
rt_uint32_t rt_current_priority;

// PendSV�жϷ�����ִ�б�־
rt_uint32_t	rt_thread_switch_interrupt_flag;
// ���ڴ洢��һ���̵߳�SPָ��						
rt_uint32_t	rt_interrupt_from_thread;
// ���ڴ洢��һ���̵߳�spָ��
rt_uint32_t	rt_interrupt_to_thread;			
			
/* ��ǰ�߳� */
struct rt_thread *rt_current_thread;
/* �߳̾����б� */
rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];

// ��ʼ��������
void rt_system_scheduler_init(void)
{
#if 0
	register rt_base_t offset;
	
	for (offset = 0; offset < RT_THREAD_PRIORITY_MAX; offset++)
	{
		rt_list_init(&rt_thread_priority_table[offset]);
	}
	
	rt_current_thread = RT_NULL;
#else 
	register rt_base_t offset;
	
	// �߳����ȼ���ʼ��
	for (offset = 0; offset < RT_THREAD_PRIORITY_MAX; offset++)
	{
		rt_list_init(&rt_thread_priority_table[offset]);
	}
	
	// ��ʼ����ǰ���ȼ�Ϊ�����߳�
	rt_current_priority = RT_THREAD_PRIORITY_MAX - 1;
	
	// ��ʼ����ǰ�߳̿��ƿ�ָ��
	rt_current_thread = RT_NULL;
	
	// ��ʼ���߳̾������ȼ���
	rt_thread_ready_priority_group = 0;
#endif
}

// ����������
void rt_system_scheduler_start(void)
{
#if 0	
	register struct rt_thread *to_thread;
	
	// �ֶ�ָ����һ�����е��߳�
	to_thread = rt_list_entry(rt_thread_priority_table[0].next,
														struct rt_thread,
														tlist);
	rt_current_thread = to_thread;
					
	/* �л�����һ���̣߳��ú����� context_rvds.S ��ʵ�֣�
		 ��rthw.h����������ʵ�ֵ�һ���߳��л���
		 ��һ����ຯ����C�ļ��е��õ�ʱ��������βΣ�
		 ��ִ�е�ʱ��Ὣ�βδ��뵽CPU�Ĵ���r0. */
	rt_hw_context_switch_to((rt_uint32_t)&to_thread->sp);
#else 
	register struct rt_thread *to_thread;
	register rt_ubase_t highest_ready_priority;
	
	// ��ȡ������������ȼ����߳�
	highest_ready_priority = __rt_ffs(rt_thread_ready_priority_group) - 1;
	
	to_thread = rt_list_entry(rt_thread_priority_table[highest_ready_priority].next,
														struct rt_thread,
														tlist);
	
	rt_current_thread = to_thread;
	
	rt_hw_context_switch_to((rt_uint32_t)&to_thread->sp);
#endif
}

void rt_schedule(void)
{
	rt_base_t level;
	register rt_ubase_t highest_ready_priority;
	struct rt_thread *to_thread;
	struct rt_thread *from_thread;
	
	level = rt_hw_interrupt_disable();
	
	highest_ready_priority = __rt_ffs(rt_thread_ready_priority_group) - 1;
	
	to_thread = rt_list_entry(rt_thread_priority_table[highest_ready_priority].next,
														struct rt_thread,
														tlist);
	
	if (to_thread != rt_current_thread)
	{
		rt_current_priority = (rt_uint8_t)highest_ready_priority;
		from_thread = rt_current_thread;
		rt_current_thread = to_thread;
		
		rt_hw_context_switch((rt_uint32_t)&from_thread->sp,
												 (rt_uint32_t)&to_thread->sp);
		
		rt_hw_interrupt_enable(level);
	}
		
	rt_hw_interrupt_enable(level);
	
	rt_hw_context_switch((rt_uint32_t)&from_thread->sp, (rt_uint32_t)&to_thread->sp);
}

// �����������߳�
void rt_schedule_insert_thread(struct rt_thread *thread)
{
	register rt_base_t temp;
	
	temp = rt_hw_interrupt_disable();
	
	thread->stat = RT_THREAD_READY;
	
	rt_list_insert_before(&(rt_thread_priority_table[thread->current_priority]),
												&(thread->tlist));
	
	// �����߳̾������ȼ����ж�Ӧ��λ
	rt_thread_ready_priority_group |= thread->number_mask;
	
	rt_hw_interrupt_enable(temp);
}

// ������ɾ���߳�
void rt_schedule_remove_thread(struct rt_thread *thread)
{
	register rt_base_t temp;
	
	temp = rt_hw_interrupt_disable();
	
	rt_list_remove(&(thread->tlist));
	
	// ���߳̾������ȼ���Ķ�Ӧ��λλ���
//	if (rt_list_isempty(&(rt_thread_priority_table[thread->current_priority])))
//	{
		rt_thread_ready_priority_group &= ~thread->number_mask;
//	}
	
	rt_hw_interrupt_enable(temp);
}

