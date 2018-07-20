	
	IMPORT	rt_thread_switch_interrupt_flag
	IMPORT	rt_interrupt_from_thread
	IMPORT	rt_interrupt_to_thread
		
;*************************************************************************
;                                 常量
;*************************************************************************
;-------------------------------------------------------------------------
;有关内核外设寄存器定义可参考官方文档：STM32F10xxx Cortex-M3 programming manual
;系统控制块外设SCB地址范围：0xE000ED00-0xE000ED3F
;-------------------------------------------------------------------------
SCB_VTOR        EQU     0xE000ED08     ; 向量表偏移寄存器
NVIC_INT_CTRL   EQU     0xE000ED04     ; 中断控制状态寄存器
NVIC_SYSPRI2    EQU     0xE000ED20     ; 系统优先级寄存器(2)
NVIC_PENDSV_PRI EQU     0x00FF0000     ; PendSV 优先级值 (lowest)
NVIC_PENDSVSET  EQU     0x10000000     ; 触发PendSV exception的值

;*************************************************************************
;                              代码产生指令
;*************************************************************************

    AREA |.text|, CODE, READONLY, ALIGN=2
    THUMB
    REQUIRE8
    PRESERVE8

;在rthw.h里面声明
;调用时，有一个参数，则传入R0，有两个参数，则另外一个传入R1

;第一次启动线程
rt_hw_context_switch_to		PROC
	
	EXPORT		rt_hw_context_switch_to
		
	LDR			r1,=rt_interrupt_to_thread
	STR			r0,[r1]
	
	;设置rt_interrupt_from_thread = 0，代表第一次启动
	LDR			r1,=rt_interrupt_from_thread
	MOV			r0,#0x0
	STR			r0,[r1]
	
	;设置rt_thread_switch_interrupt_flag = 1
	LDR			r1,=rt_thread_switch_interrupt_flag
	MOV			r0,#1
	STR			r0,[r1]
	
	;设置 PendSV 优先级
	LDR			r0,=NVIC_SYSPRI2
	LDR			r1,=NVIC_PENDSV_PRI
	LDR.W		r2,[r0,#0x00]
	ORR			r1,r1,r2
	STR			r1,[r0]
	
	LDR			r0,=NVIC_INT_CTRL
	LDR			r1,=NVIC_PENDSVSET
	STR			r1,[r0]

	CPSIE		F
	CPSIE		I
	
	ENDP

rt_hw_context_switch	PROC
	
	EXPORT		rt_hw_context_switch
				
	LDR			r2,=rt_thread_switch_interrupt_flag
	LDR			r3,[r2]
	CMP			r3,#1
	BEQ			_reswitch
	MOV			r3,#1
	STR			r3,[r2]
	
	LDR			r2,=rt_interrupt_from_thread
	STR			r0,[r2]
	
_reswitch
	LDR			r2,=rt_interrupt_to_thread
	STR			r1,[r2]
	
	LDR			r0,=NVIC_INT_CTRL
	LDR			r1,=NVIC_PENDSVSET
	STR			r1,[r0]
	
	BX			LR
	
	ENDP
	
;/*
; *-----------------------------------------------------------------------
; * void PendSV_Handler(void);
; * r0 --> switch from thread stack
; * r1 --> switch to thread stack
; * psr, pc, lr, r12, r3, r2, r1, r0 are pushed into [from] stack
; *-----------------------------------------------------------------------
; */
PendSV_Handler	PROC
	EXPORT		PendSV_Handler

	MRS			r2,PRIMASK
	CPSID		I
	
	LDR			r0,=rt_thread_switch_interrupt_flag
	LDR			r1,[r0]
	CBZ			r1,pendsv_exit

	MOV			r1,#0x00
	STR			r1,[r0]	
		
	LDR			r0,=rt_interrupt_from_thread
	LDR			r1,[r0]
	;rt_interrupt_from_thread = 0, 跳转 switch_to_thread
	CBZ			r1,switch_to_thread
	
	
	MRS			r1,psp
	STMFD		r1!,{r4-r11}
	LDR			r0,[r0]
	STR			r1,[r0]

switch_to_thread
	
	LDR			r1,=rt_interrupt_to_thread
	LDR			r1,[r1]
	LDR			r1,[r1]
	
	LDMFD		r1!,{r4-r11}
	MSR			psp,r1

pendsv_exit
	MSR			PRIMASK,r2
	
	ORR			lr,lr,#0x04

	BX			lr
	
	ENDP
		
	ALIGN		4
	
	END



















