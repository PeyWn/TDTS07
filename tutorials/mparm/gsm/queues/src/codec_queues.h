/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */
#ifndef _CODEC_QUEUES_H
#define _CODEC_QUEUES_H

#include	"private.h"
#include	"gsm.h"
#include	"proto.h"


//#include        "sasa_support.h"

struct encoder_queue {
  //  struct gsm_state	* en_qu_S;
  word    en_qu_Nc[4];    /* [0..3] LTP lag                       OUT     */
  word    en_qu_bc[4];    /* [0..3] coded LTP gain                OUT     */
  word    en_qu_Mc[4];    /* [0..3] RPE grid selection            OUT     */
  word    en_qu_xmaxc[4]; /* [0..3] Coded maximum amplitude       OUT     */
  word    en_qu_xMc[13*4];   /* [13*4] normalized RPE samples        OUT     */
  word    en_qu_LARc[8];  /* [0..7] LAR coefficients              OUT     */
  word    en_qu_e[50];     /*e[50]*/
  word    en_qu_so[160];    /*s[160]*/
};

struct decoder_queue {
  //  struct gsm_state	* de_s1_S;
  word		 de_qu_LARcr[8];	/* [0..7]		IN	*/
  word		 de_qu_Ncr[4];	/* [0..3] 		IN 	*/
  word		 de_qu_bcr[4];	/* [0..3]		IN	*/
  word		 de_qu_Mcr[4];	/* [0..3] 		IN 	*/
  word		 de_qu_xmaxcr[4];	/* [0..3]		IN 	*/
  word		 de_qu_xMcr[13*4];	/* [0..13*4]		IN	*/
  word		 de_qu_s[160];         /* [0..159]		OUT 	*/
  word		 de_qu_erp[40];       /*40*/
  word           de_qu_wt[160];        /*[160]*/
  word		 de_qu_drp;
};


#endif


