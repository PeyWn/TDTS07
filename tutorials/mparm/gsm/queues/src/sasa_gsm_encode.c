/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */

/* $Header: /tmp_amd/presto/export/kbs/jutta/src/gsm/RCS/gsm_encode.c,v 1.2 1996/07/02 09:59:05 jutta Exp $ */

#include "private.h"
#include "gsm.h"
#include "proto.h"

#ifdef SASA_MPARM
#include "appsupport.h"
#endif

extern word    * shared_Nc_1;    /* [0..3] LTP lag                       OUT     */
extern word    * shared_bc_1;    /* [0..3] coded LTP gain                OUT     */
extern word    * shared_Mc_1;    /* [0..3] RPE grid selection            OUT     */
extern word    * shared_xmaxc_1; /* [0..3] Coded maximum amplitude       OUT     */
extern word    * shared_xMc_1;   /* [13*4] normalized RPE samples        OUT     */
extern word    * shared_LARc_1;  /* [0..7] LAR coefficients              OUT     */

extern word    * shared_Nc_2;    /* [0..3] LTP lag                       OUT     */
extern word    * shared_bc_2;    /* [0..3] coded LTP gain                OUT     */
extern word    * shared_Mc_2;    /* [0..3] RPE grid selection            OUT     */
extern word    * shared_xmaxc_2; /* [0..3] Coded maximum amplitude       OUT     */
extern word    * shared_xMc_2;   /* [13*4] normalized RPE samples        OUT     */
extern word    * shared_LARc_2;  /* [0..7] LAR coefficients              OUT     */



extern word    * shared_Nc;    /* [0..3] LTP lag                       OUT     */
extern word    * shared_bc;    /* [0..3] coded LTP gain                OUT     */
extern word    * shared_Mc;    /* [0..3] RPE grid selection            OUT     */
extern word    * shared_xmaxc; /* [0..3] Coded maximum amplitude       OUT     */
extern word    * shared_xMc;   /* [13*4] normalized RPE samples        OUT     */
extern word    * shared_LARc;  /* [0..7] LAR coefficients              OUT     */

extern int	*sasa_sem;

void new_locals_to_shared(word* LARc, word* Nc, word* bc, word* Mc, word* xmaxc, word* xMc) {
 int i;
 for (i=0;i<8;i++) shared_LARc[i] = LARc[i];
 for (i=0;i<4;i++) shared_Nc[i] = Nc[i];
 for (i=0;i<4;i++) shared_bc[i] = bc[i];
 for (i=0;i<4;i++) shared_Mc[i] = Mc[i];
 for (i=0;i<4;i++) shared_xmaxc[i] = xmaxc[i];
 for (i=0;i<13*4+1;i++) shared_xMc[i] = xMc[i]; 
}


void new_shared_to_locals(word* LARc, word* Nc, word* bc, word* Mc, word* xmaxc, word* xMc) {
 int i;
 for (i=0;i<8;i++) LARc[i] = shared_LARc[i];
 for (i=0;i<4;i++) Nc[i] = shared_Nc[i];
 for (i=0;i<4;i++) bc[i] = shared_bc[i];
 for (i=0;i<4;i++) Mc[i] = shared_Mc[i];
 for (i=0;i<4;i++) xmaxc[i] = shared_xmaxc[i];
 for (i=0;i<13*4+1;i++) xMc[i] = shared_xMc[i]; 
}

void gsm_encode P3((s, source, c), gsm s, gsm_signal * source, gsm_byte * c)
{
	word	 	LARc[8], Nc[4], Mc[4], bc[4], xmaxc[4], xmc[13*4];
	
	Gsm_Coder(s, source, LARc, Nc, bc, Mc, xmaxc, xmc);

	if (get_id()==2) 
	
	{
		
		
		*c++ =   ((GSM_MAGIC & 0xF) << 4)		/* 1 */
		       | ((LARc[0] >> 2) & 0xF);
		*c++ =   ((LARc[0] & 0x3) << 6)
		       | (LARc[1] & 0x3F);
		*c++ =   ((LARc[2] & 0x1F) << 3)
		       | ((LARc[3] >> 2) & 0x7);
		*c++ =   ((LARc[3] & 0x3) << 6)
		       | ((LARc[4] & 0xF) << 2)
		       | ((LARc[5] >> 2) & 0x3);
		*c++ =   ((LARc[5] & 0x3) << 6)
		       | ((LARc[6] & 0x7) << 3)
		       | (LARc[7] & 0x7);
		*c++ =   ((Nc[0] & 0x7F) << 1)
		       | ((bc[0] >> 1) & 0x1);
		*c++ =   ((bc[0] & 0x1) << 7)
		       | ((Mc[0] & 0x3) << 5)
		       | ((xmaxc[0] >> 1) & 0x1F);
		*c++ =   ((xmaxc[0] & 0x1) << 7)
		       | ((xmc[0] & 0x7) << 4)
		       | ((xmc[1] & 0x7) << 1)
		       | ((xmc[2] >> 2) & 0x1);
		*c++ =   ((xmc[2] & 0x3) << 6)
		       | ((xmc[3] & 0x7) << 3)
		       | (xmc[4] & 0x7);
		*c++ =   ((xmc[5] & 0x7) << 5)			/* 10 */
		       | ((xmc[6] & 0x7) << 2)
		       | ((xmc[7] >> 1) & 0x3);
		*c++ =   ((xmc[7] & 0x1) << 7)
		       | ((xmc[8] & 0x7) << 4)
		       | ((xmc[9] & 0x7) << 1)
		       | ((xmc[10] >> 2) & 0x1);
		*c++ =   ((xmc[10] & 0x3) << 6)
		       | ((xmc[11] & 0x7) << 3)
		       | (xmc[12] & 0x7);
		*c++ =   ((Nc[1] & 0x7F) << 1)
		       | ((bc[1] >> 1) & 0x1);
		*c++ =   ((bc[1] & 0x1) << 7)
		       | ((Mc[1] & 0x3) << 5)
		       | ((xmaxc[1] >> 1) & 0x1F);
		*c++ =   ((xmaxc[1] & 0x1) << 7)
		       | ((xmc[13] & 0x7) << 4)
		       | ((xmc[14] & 0x7) << 1)
		       | ((xmc[15] >> 2) & 0x1);
		*c++ =   ((xmc[15] & 0x3) << 6)
		       | ((xmc[16] & 0x7) << 3)
		       | (xmc[17] & 0x7);
		*c++ =   ((xmc[18] & 0x7) << 5)
		       | ((xmc[19] & 0x7) << 2)
		       | ((xmc[20] >> 1) & 0x3);
		*c++ =   ((xmc[20] & 0x1) << 7)
		       | ((xmc[21] & 0x7) << 4)
		       | ((xmc[22] & 0x7) << 1)
		       | ((xmc[23] >> 2) & 0x1);
		*c++ =   ((xmc[23] & 0x3) << 6)
		       | ((xmc[24] & 0x7) << 3)
		       | (xmc[25] & 0x7);
		*c++ =   ((Nc[2] & 0x7F) << 1)			/* 20 */
		       | ((bc[2] >> 1) & 0x1);
		*c++ =   ((bc[2] & 0x1) << 7)
		       | ((Mc[2] & 0x3) << 5)
		       | ((xmaxc[2] >> 1) & 0x1F);
		*c++ =   ((xmaxc[2] & 0x1) << 7)
		       | ((xmc[26] & 0x7) << 4)
		       | ((xmc[27] & 0x7) << 1)
		       | ((xmc[28] >> 2) & 0x1);
		*c++ =   ((xmc[28] & 0x3) << 6)
		       | ((xmc[29] & 0x7) << 3)
		       | (xmc[30] & 0x7);
		*c++ =   ((xmc[31] & 0x7) << 5)
		       | ((xmc[32] & 0x7) << 2)
		       | ((xmc[33] >> 1) & 0x3);
		*c++ =   ((xmc[33] & 0x1) << 7)
		       | ((xmc[34] & 0x7) << 4)
		       | ((xmc[35] & 0x7) << 1)
		       | ((xmc[36] >> 2) & 0x1);
		*c++ =   ((xmc[36] & 0x3) << 6)
		       | ((xmc[37] & 0x7) << 3)
		       | (xmc[38] & 0x7);
		*c++ =   ((Nc[3] & 0x7F) << 1)
		       | ((bc[3] >> 1) & 0x1);
		*c++ =   ((bc[3] & 0x1) << 7)
		       | ((Mc[3] & 0x3) << 5)
		       | ((xmaxc[3] >> 1) & 0x1F);
		*c++ =   ((xmaxc[3] & 0x1) << 7)
		       | ((xmc[39] & 0x7) << 4)
		       | ((xmc[40] & 0x7) << 1)
		       | ((xmc[41] >> 2) & 0x1);
		*c++ =   ((xmc[41] & 0x3) << 6)			/* 30 */
		       | ((xmc[42] & 0x7) << 3)
		       | (xmc[43] & 0x7);
		*c++ =   ((xmc[44] & 0x7) << 5)
		       | ((xmc[45] & 0x7) << 2)
		       | ((xmc[46] >> 1) & 0x3);
		*c++ =   ((xmc[46] & 0x1) << 7)
		       | ((xmc[47] & 0x7) << 4)
		       | ((xmc[48] & 0x7) << 1)
		       | ((xmc[49] >> 2) & 0x1);
		*c++ =   ((xmc[49] & 0x3) << 6)
		       | ((xmc[50] & 0x7) << 3)
		       | (xmc[51] & 0x7);
		//new_locals_to_shared(LARc, Nc, bc, Mc, xmaxc, xmc);
		pr(get_id(),5,888);
		sasa_sem[0]=0;
	}
}
