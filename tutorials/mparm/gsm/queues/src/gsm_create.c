/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */

static char const	ident[] = "$Header: /tmp_amd/presto/export/kbs/jutta/src/gsm/RCS/gsm_create.c,v 1.4 1996/07/02 09:59:05 jutta Exp $";

#include	"config.h"

#ifdef	HAS_STRING_H
#include	<string.h>
#else
#	include "proto.h"
	extern char	* memset P((char *, int, int));
#endif

#ifdef	HAS_STDLIB_H
#	include	<stdlib.h>
#else
#	ifdef	HAS_MALLOC_H
#		include 	<malloc.h>
#	else
		extern char * malloc();
#	endif
#endif

#include <stdio.h>

#include "gsm.h"
#include "private.h"
#include "proto.h"

extern int sasa_malloc_counter;
gsm gsm_create P0()
{
	gsm  r;
#ifndef SASA_MPARM
	r = (gsm)malloc(sizeof(struct gsm_state));
	if (!r) {
	/*	pr(0,2,-1111);*/
		return r;
	}
#else
	r = (void *)SHARED_BASE + sasa_malloc_counter; /*mmmmm ptr?*/
	sasa_malloc_counter += sizeof(struct gsm_state);
#endif
	/*	memset((char *)r, 0, sizeof(*r));*/
	r->nrp = 40;
	return r;
}
