/* ld script to make ARM Linux kernel
 * taken from the i386 version by Russell King
 * Written by Martin Mares <mj@atrey.karlin.mff.cuni.cz>
 */
OUTPUT_ARCH(arm)
/*ENTRY(stext)*/
SECTIONS
{
	. = 0;
	.vectors1 : {
	        *(.resetvector1)
	}
	. = 0x18;
	.irq_vector : {
	        *(.irqvector)
	}
        . = 0x8000;
	.init : {			/* Init code and data		*/
/*		_stext = .;       */
/*		__init_begin = .;*/
			*(.text1.init)
			*(.text2.init)
/*		__init_end = .;  */
	}

	/DISCARD/ : {			/* Exit code and data		*/
		*(.text.exit)
		*(.data.exit)
		*(.exitcall.exit)
	}

	.text : {			/* Real text segment		*/
		_text = .;		/* Text and read-only data	*/
			*(.text)
		_etext = .;		/* End of text section		*/
	}

	. = ALIGN(8192);

	.data : {
		/*
		 * first, the init task union, aligned
		 * to an 8192 byte boundary.
		 */
		*(.init.task)

		/*
		 * then the cacheline aligned data
		 */
		. = ALIGN(32);
		*(.data.cacheline_aligned)

		/*
		 * and the usual data section
		 */
		*(.data)
		CONSTRUCTORS

		_edata = .;
	}

	.bss : {
		__bss_start = .;	/* BSS				*/
		*(.bss)
		*(COMMON)
		_end = . ;
	}
        
        . = 0x400000;
	.stack1 : {
	        __stack_start1 = . ;
	}
        . = 0x800000;
	.stack2 : {
		__stack_start2 = . ;
	}
        
	. = ALIGN(8192);
	_end_kernel = . ;

	.vectors2 : {
	        *(.resetvector2)
	}
	
					/* Stabs debugging sections.	*/
/*	.stab 0 : { *(.stab) }
	.stabstr 0 : { *(.stabstr) }
	.stab.excl 0 : { *(.stab.excl) }
	.stab.exclstr 0 : { *(.stab.exclstr) }
	.stab.index 0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment 0 : { *(.comment) }
	.debug_abbrev 0 : { *(.debug_abbrev) }
	.debug_info 0 : { *(.debug_info) }
	.debug_line 0 : { *(.debug_line) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	.debug_aranges 0 : { *(.debug_aranges) } */
}
