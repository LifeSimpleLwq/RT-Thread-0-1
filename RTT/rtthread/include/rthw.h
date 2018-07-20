#ifndef __RT_HW_H__
#define __RT_HW_H__

#include <rtthread.h>


/*
*************************************************************************
*                               �������л���غ�������
*************************************************************************
*/
void rt_hw_context_switch(rt_uint32_t from, rt_uint32_t to);
void rt_hw_context_switch_to(rt_uint32_t to);
void rt_hw_context_switch_interrupt(rt_uint32_t from, rt_uint32_t to);



#endif
