/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
*/

/* $Header: /tmp_amd/presto/export/kbs/jutta/src/gsm/RCS/code.c,v 1.3 1996/07/02 09:59:05 jutta Exp $ */

#include	"config.h"


#ifdef	HAS_STDLIB_H
#include	<stdlib.h>
#else
#	include "proto.h"
	extern char	* memcpy P((char *, char *, int));
#endif

#include	"private.h"
#include	"gsm.h"
#include	"proto.h"

/* 
 *  4.2 FIXED POINT IMPLEMENTATION OF THE RPE-LTP CODER 
 */

#ifdef SASA_MPARM
#include "appsupport.h"
#endif

extern word    * shared_Nc_1;    /* [0..3] LTP lag                       OUT     */
extern word    * shared_bc_1;    /* [0..3] coded LTP gain                OUT     */
extern word    * shared_Mc_1;    /* [0..3] RPE grid selection            OUT     */
extern word    * shared_xmaxc_1; /* [0..3] Coded maximum amplitude       OUT     */
extern word    * shared_xMc_1;   /* [13*4] normalized RPE samples        OUT     */
extern word    * shared_LARc_1;  /* [0..7] LAR coefficients              OUT     */
extern word    * shared_e_1;     /*e[50]*/
extern word    * shared_so_1;    /*s[160]*/


extern word    * shared_Nc_2;    /* [0..3] LTP lag                       OUT     */
extern word    * shared_bc_2;    /* [0..3] coded LTP gain                OUT     */
extern word    * shared_Mc_2;    /* [0..3] RPE grid selection            OUT     */
extern word    * shared_xmaxc_2; /* [0..3] Coded maximum amplitude       OUT     */
extern word    * shared_xMc_2;   /* [13*4] normalized RPE samples        OUT     */
extern word    * shared_LARc_2;  /* [0..7] LAR coefficients              OUT     */
extern word    * shared_e_2;     /*e[50]*/
extern word    * shared_so_2;    /*s[160]*/


extern word    * shared_Nc;    /* [0..3] LTP lag                       OUT     */
extern word    * shared_bc;    /* [0..3] coded LTP gain                OUT     */
extern word    * shared_Mc;    /* [0..3] RPE grid selection            OUT     */
extern word    * shared_xmaxc; /* [0..3] Coded maximum amplitude       OUT     */
extern word    * shared_xMc;   /* [13*4] normalized RPE samples        OUT     */
extern word    * shared_LARc;  /* [0..7] LAR coefficients              OUT     */
extern word    * shared_e;     /*e[50]*/
extern word    * shared_so;    /*s[160]*/

extern struct gsm_state * shared_S;
extern word    * shared_s;
extern int     *shared_buffer_lock;

void
copy_buffer_to_mem2() {
 int i;
 pr(get_id(), 5, 65);
 while (shared_buffer_lock[0] == 0) ; //while EMPTY wait
 pr(get_id(), 5, 66);
 for (i=0;i<8;i++) shared_LARc_2[i] = shared_LARc[i];
 for (i=0;i<4;i++) shared_Nc_2[i] = shared_Nc[i];
 for (i=0;i<4;i++) shared_bc_2[i] = shared_bc[i];
 for (i=0;i<4;i++) shared_Mc_2[i] = shared_Mc[i];
 for (i=0;i<4;i++) shared_xmaxc_2[i]= shared_xmaxc[i];
 for (i=0;i<13*4;i++) shared_xMc_2[i] = shared_xMc[i];
 for (i=0;i<50;i++) shared_e_2[i] = shared_e[i];
 for (i=0;i<160;i++) shared_so_2[i] = shared_so[i];
 shared_buffer_lock[0] = 0;
}

void
copy_mem1_to_buffer() {
 int i;
 pr(get_id(), 5, 65);
 while (shared_buffer_lock[0] == 1) ; //while FULL wait
 pr(get_id(), 5, 66);
 for (i=0;i<8;i++) shared_LARc[i] = shared_LARc_1[i];
 for (i=0;i<4;i++) shared_Nc[i] = shared_Nc_1[i];
 for (i=0;i<4;i++) shared_bc[i] = shared_bc_1[i];
 for (i=0;i<4;i++) shared_Mc[i] = shared_Mc_1[i];
 for (i=0;i<4;i++) shared_xmaxc[i]= shared_xmaxc_1[i];
 for (i=0;i<13*4;i++) shared_xMc[i] = shared_xMc_1[i];
 for (i=0;i<50;i++) shared_e[i] = shared_e_1[i];
 for (i=0;i<160;i++) shared_so[i] = shared_so_1[i];
 shared_buffer_lock[0] = 1;
}


void locals_to_shared(word* LARc, word* Nc, word* bc, word* Mc, word* xmaxc, word* xMc, word *e, word *so, 
		      struct gsm_state * S, word *s) {
 int i;

 for (i=0;i<8;i++) shared_LARc_1[i] = LARc[i];
 for (i=0;i<4;i++) shared_Nc_1[i] = Nc[i];
 for (i=0;i<4;i++) shared_bc_1[i] = bc[i];
 for (i=0;i<4;i++) shared_Mc_1[i] = Mc[i];
 for (i=0;i<4;i++) shared_xmaxc_1[i] = xmaxc[i];
 for (i=0;i<13*4;i++) shared_xMc_1[i] = xMc[i];
 for (i=0;i<50;i++) shared_e_1[i] = e[i];
 for (i=0;i<160;i++) shared_so_1[i] = so[i];

 //for (i=0;i<160;i++) shared_s[i] = s[i]; 
}


void shared_to_locals(word* LARc, word* Nc, word* bc, word* Mc, word* xmaxc, word* xMc, word *e, word *so, 
		      struct gsm_state * S, word *s) {
 int i;

 for (i=0;i<8;i++) LARc[i] = shared_LARc_2[i];
 for (i=0;i<4;i++) Nc[i] = shared_Nc_2[i];
 for (i=0;i<4;i++) bc[i] = shared_bc_2[i];
 for (i=0;i<4;i++) Mc[i] = shared_Mc_2[i];
 for (i=0;i<4;i++) xmaxc[i] = shared_xmaxc_2[i];
 for (i=0;i<13*4;i++) xMc[i] = shared_xMc_2[i];
 for (i=0;i<50;i++)  e[i] = shared_e_2[i];
 for (i=0;i<160;i++) so[i] = shared_so_2[i];

 //for (i=0;i<160;i++) shared_s[i] = s[i];
}

void Gsm_Coder P8((S,s,LARc,Nc,bc,Mc,xmaxc,xMc),

	struct gsm_state	* S,

	word	* s,	/* [0..159] samples		  	IN	*/

/*
 * The RPE-LTD coder works on a frame by frame basis.  The length of
 * the frame is equal to 160 samples.  Some computations are done
 * once per frame to produce at the output of the coder the
 * LARc[1..8] parameters which are the coded LAR coefficients and 
 * also to realize the inverse filtering operation for the entire
 * frame (160 samples of signal d[0..159]).  These parts produce at
 * the output of the coder:
 */

	word	* LARc,	/* [0..7] LAR coefficients		OUT	*/

/*
 * Procedure 4.2.11 to 4.2.18 are to be executed four times per
 * frame.  That means once for each sub-segment RPE-LTP analysis of
 * 40 samples.  These parts produce at the output of the coder:
 */

	word	* Nc,	/* [0..3] LTP lag			OUT 	*/
	word	* bc,	/* [0..3] coded LTP gain		OUT 	*/
	word	* Mc,	/* [0..3] RPE grid selection		OUT     */
	word	* xmaxc,/* [0..3] Coded maximum amplitude	OUT	*/
	word	* xMc	/* [13*4] normalized RPE samples	OUT	*/
)
{
	int	k;
	word	* dp  = S->dp0 + 120;	/* [ -120...-1 ] */
	word	* dpp = dp;		/* [ 0...39 ]	 */

	static word e[50];

	word	so[160];
	
        word    * tmp_Nc;   /* [0..3] LTP lag                       OUT     */
        word    * tmp_bc;   /* [0..3] coded LTP gain                OUT     */
        word    * tmp_Mc;   /* [0..3] RPE grid selection            OUT     */
        word    * tmp_xmaxc;/* [0..3] Coded maximum amplitude       OUT     */
        word    * tmp_xMc;   /* [13*4] normalized RPE samples        OUT     */


	/*pr(0,2,-20);		*/
	if (get_id()==1) {
 	  pr(get_id(),5,6563);
	  //while(semaphores[6]==1) ;
	  pr(get_id(),5,6564);
	}
	if (get_id()==1) {
	  Gsm_Preprocess		(S, s, so);
	}
	/*pr(0,2,-21);*/
	if (get_id()==1) {
	  Gsm_LPC_Analysis		(S, so, LARc);
	}
	/*pr(0,2,-22);*/
	if (get_id()==1) {
	  int i;
	  Gsm_Short_Term_Analysis_Filter	(S, LARc, so);
	  locals_to_shared(LARc, Nc, bc, Mc, xmaxc, xMc, e, so, S, s);
	  semaphores[18]=0;
	  copy_mem1_to_buffer();
	  pr(get_id(),5,974);
	  shared_buffer_lock[0] =1; 
	}

	if (get_id()==2) {	
  	  int i;
	  pr(get_id(), 5, 76);
	  while (shared_buffer_lock[0] == 0) ; 
	  copy_buffer_to_mem2();
	  pr(get_id(), 5, 77);
	  shared_to_locals(LARc, Nc, bc, Mc, xmaxc, xMc, e, so, S, s);
	  tmp_Nc = Nc;
	  tmp_bc = bc;
	  tmp_Mc = Mc;
	  tmp_xmaxc = xmaxc;
	  tmp_xMc = xMc;
	  for (k = 0; k <= 3; k++, xMc += 13) {
		int i;
		pr(get_id(),2,45+k);
		Gsm_Long_Term_Predictor	( S,
					 so+k*40, /* d      [0..39] IN	*/
					 dp,	  /* dp  [-120..-1] IN	*/
					e + 5,	  /* e      [0..39] OUT	*/
					dpp,	  /* dpp    [0..39] OUT */
					 Nc++,
					 bc++);

		Gsm_RPE_Encoding	( S,
					  e + 5,  /* e	  ][0..39][ IN/OUT */
					  xmaxc++, Mc++, xMc );
		
		/*
		 * Gsm_Update_of_reconstructed_short_time_residual_signal
		 *			( dpp, e + 5, dp );
		 */
		
		{ register int i;
		  register longword ltmp;
		  for (i = 0; i <= 39; i++)
			dp[ i ] = GSM_ADD( e[5 + i], dpp[i] );
		}
		dp  += 40;
		dpp += 40;

	  }
	  (void)memcpy( (char *)S->dp0, (char *)(S->dp0 + 160),
		120 * sizeof(*S->dp0) );
	
	  //locals_to_shared(LARc, tmp_Nc, tmp_bc, tmp_Mc, tmp_xmaxc, tmp_xMc, e, so, S, s);
	  pr(get_id(),5,-28);
	
	  //semaphores[6]=0;
	}
}
