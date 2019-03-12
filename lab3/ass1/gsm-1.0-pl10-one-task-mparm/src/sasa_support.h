#ifndef SASA_SUPPORT_H
#define SASA_SUPPORT_H

#ifndef OLD_MPARM

#include "sim_support_flags.h"

#define PR_INT(z) pr(NULL, z, PR_CPU_ID | PR_DEC | PR_NEWL )
#define PR_STR(x)  pr(x, 0, PR_CPU_ID | PR_STRING | PR_NEWL );
//#define MARCUS_DEBUG_MESSAGE(x) ;
#define get_id get_proc_id

#else
#define get_id get_id
#define PR(x,y,z) pr(get_id(), y, z)
#define MARCUS_DEBUG_MESSAGE(x)  pr(get_id(), 10, x);
#define dump_light_metric
#endif /*OLD_MPARM*/

#endif /*SASA_SUPPORT_H*/
