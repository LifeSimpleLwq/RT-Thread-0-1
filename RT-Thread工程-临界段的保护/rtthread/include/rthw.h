#ifndef __RT_HW_H__
#define __RT_HW_H__

#include "rtdef.h"

void rt_hw_context_switch(rt_uint32_t from, rt_uint32_t to);
void rt_hw_context_switch_to(rt_uint32_t to);

void rt_hw_interrupt_enable(void);
void rt_hw_interrupt_disable(void);

#endif /* __RT_HW_H__ */
