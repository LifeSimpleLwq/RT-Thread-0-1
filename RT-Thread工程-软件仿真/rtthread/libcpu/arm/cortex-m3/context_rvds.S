;**************************************************
;				全局变量
;**************************************************
	IMPORT	rt_thread_switch_interrupt_flag
	IMPORT	rt_interrupt_from_thread
	IMPORT	rt_interrupt_to_thread

;**************************************************
;				常量
;**************************************************
;有关内核外设寄存器定义可参考官方文档：STM32F10XXX Cortex-M3 Programming manual
;系统控制块外设SCB地址范围：0xE000ED00-0XE000ED3F
;-------------------------------------------------
SCB_VTOR		EQU			0xE000ED08
NVIC_INT_CTRL	EQU	 		0xE000ED04
NVIC_SYSPRI2	EQU	 		0xE000ED20
NVIC_PENDSV_PRI	EQU	 		0x00FF0000
NVIC_PENDSVSET	EQU	 		0x10000000

;**************************************************
;				代码产生指令
;**************************************************
	AREA |.text|, code, READONLY, ALIGN=2
	THUMB
	REQUIRE8
	PRESERVE8


;**************************************************
;	函数原型： void rt_hw_context_switch_to(rt_uint32 to);
;	r0 -> to
; 	该函数用于开启第一次线程切换
;**************************************************
rt_hw_context_switch_to		PROC
	
	;	导出rt_hw_context_switch_to， 让其具有全局属性，可以在C文件调用
	EXPORT	rt_hw_context_switch_to
	
	
	;	将rt_interrupt_to_thread的地址加载到r1
	LDR		r1, =rt_interrupt_to_thread
	;	将r0的值存储到rt_interrupt_to_thread
	STR		r0, [r1]
	
	
	;	设置rt_interrupt_from_thread的值为0，表示启动第一次线程切换
	; 	将rt_interrupt_from_thread的地址加载到r1
	LDR		r1, =rt_interrupt_from_thread
	; 	配置r0 = 0
	MOV 	r0, #0x0
	;	将r0的值存储到rt_interrupt_from_thread
	STR		r0, [r1]
	
	
	;	设置rt_thread_switch_interrupt_flag的值为1
	;	将rt_thread_switch_interrupt_flag的地址加载到r1
	LDR		r1, =rt_thread_switch_interrupt_flag
	; 	配置r0 = 1
	MOV		r0, #1
	;	将r0的值存储到rt_thread_switch_interrupt_flag
	STR		r0, [r1]
	
	; 	设置PendSV 异常的优先级
	LDR		r0, =NVIC_SYSPRI2
	LDR		r1,	=NVIC_PENDSV_PRI
	LDR.W	r2,	[r0, #0x00]
	ORR		r1, r1, r2
	STR		r1, [r0]
	
	;	触发 PendSV异常 (产生上下文切换)
	LDR 	r0, =NVIC_INT_CTRL
	LDR		r1, =NVIC_PENDSVSET
	STR		r1, [r0]
	
	;	开中断
	CPSIE	F
	CPSIE	I
	
	ENDP

		
;**************************************************
;	void PendSV_Handler(void);
;	r0 --> switch from thread stack
; 	r1 --> switch to thread stack
;	psr, pc, lr, r12, r3, r2, r1, r0 are pushed into [from] stack
;**************************************************
PendSV_Handler	PROC		
	EXPORT		PendSV_Handler
	
	;	失能中断，为了保护上下文切换不被中断
	MRS			r2,	PRIMASK
	CPSID		I
	
	;	获取中断标志位，看看是否为0
	; 	加载rt_thread_switch_interrupt_flag的地址到r0
	LDR			r0, =rt_thread_switch_interrupt_flag
	; 	加载rt_thread_switch_interrupt_flag的地址到r1
	LDR			r1, [r0]
	;	判断r1是否为0，为0则跳转到pendsv_exit
	CBZ			r1, pendsv_exit
	
	;	r1 != 0, r1 = 0
	MOV			r1, #0x00
	;	将r1的值加载到rt_thread_switch_interrupt_flag
	STR			r1, [r0]
	
	
	;	判断rt_interrupt_from_thread的值是否为0
	;	加载rt_interrupt_from_thread到r0
	LDR			r0, =rt_interrupt_from_thread
	LDR			r1, [r0]
	CBZ			r1, switch_to_thread
	
;===================== 上文保存 =========================
;	当进入 PendSV_Handler 时，上一个线程运行的环境即：
;	xPSR, PC, R14, R12, R3, R2, R1, R0
;	这些CPU寄存器的值会自动保存到线程的栈中，剩下r4~r11需要手动保存
;	获取线程栈指针到r1
	MRS			r1, psp
	; 	将CPU寄存器r4~r11的值存储到r1指向的地址
	STMFD		r1!, {r4 - r11}
	; 	加载r0指向值到r0，即r0=rt_interrupt_from_thread
	LDR			r0, [r0]
	STR			r1, [r0]

;===================== 下文切换 =========================
switch_to_thread
	
	LDR			r1, =rt_interrupt_to_thread
	LDR			r1, [r1]
	LDR			r1, [r1]

	LDMFD 		r1!, {r4-r11}
	MSR			psp, r1

pendsv_exit
	
	MSR			PRIMASK, r2
	ORR			lr, lr, #0x04

	BX			lr

	ENDP


;**************************************************
;	void rt_hw_context_switch(rt_uint32_t from, rt_uint32_t to);
;**************************************************
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

;============= 文件结束 ===========
	ALIGN 	4

	END

























