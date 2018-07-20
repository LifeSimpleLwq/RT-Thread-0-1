#ifndef __RT_HW_H__
#define __RT_HW_H__

#include <rtthread.h>


/*
*************************************************************************
*                               上下文切换相关函数声明
*************************************************************************
*/
void rt_hw_context_switch(rt_uint32_t from, rt_uint32_t to);
void rt_hw_context_switch_to(rt_uint32_t to);
void rt_hw_context_switch_interrupt(rt_uint32_t from, rt_uint32_t to);



#endif

