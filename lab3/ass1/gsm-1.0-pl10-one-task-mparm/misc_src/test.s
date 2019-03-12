/*
 *  Kernel startup entry point.
 *
 * The rules are:
 *  r0      - should be 0
 *  r1      - unique architecture number
 *  MMU     - off
 *  I-cache - on or off
 *  D-cache - off
 *
 * See linux/arch/arm/tools/mach-types for the complete list of numbers
 * for r1.
 */

.global  	__exit_simul

.section ".text1.init",#alloc,#execinstr
		.type	stext, #function 


__start1:
		         /* Enter IRQ mode and set up the IRQ stack pointer */
		         /* #Mode_IRQ | I_Bit | F_Bit */ /* No interrupts */
		mov     r0, #0x12     | 0x80  | 0x40     /* No interrupts */
		msr     cpsr, r0
		ldr     sp, _irq_data

		mrs   r0, cpsr          @ Get the CPSR
		bic   r0, r0, #0x1F     @ Remove current mode
		orr   r0, r0, #0x10     @ Go to user mode
		msr   cpsr_c, r0        @ Set the mode  

		/* set stack pointer and bss limits */
		adr	r3, __switch_data
		ldmia	r3, {r4, r5, sp}@       	@ sp = stack pointer

		/* enable interrupts */
		mrs   r0, cpsr          @ Get the CPSR
		bic   r0, r0, #0x1F     @ Remove current mode
		orr   r0, r0, #0x10     @ Go to user mode
		bic   r0, r0, #0xC0     @ Enable IRQ and FIQ
		msr   cpsr_c, r0        @ Set the mode  

		/* enable extint line of the interrupt controller  */
		/* (*0x90050000) = 0x800000 */
		mov     r3, #0x90000004
		add     r3, r3, #0x50000
		mov     r2, #0x800000
		str     r2, [r3, #0]
	


		mov	fp, #0				@ Clear BSS (and zero fp)
1:		cmp	r4, r5
		strcc	fp, [r4],#4
		bcc	1b

		bl	main1

loop:		b loop


__switch_data:	.long	__bss_start
		.long	_end
		.long	__stack_start1

_irq_data:
		.long	__stack_start2


.section ".resetvector1",#alloc,#execinstr
		.type	stext, #function
		b	__start1

.section ".irqvector",#alloc,#execinstr
		.type	stext, #function
	ldr	pc, .interrupt_function_addr

.L4:
	.align	2
.interrupt_function_addr:
	.word	interrupt_function



.section ".text",#alloc,#execinstr
		.type	stext, #function
__exit_simul:	mov     r0, r0
                mov     r0, r0
__end_loop:	b	__end_loop



