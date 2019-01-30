/*
///////////////////////////////////////////////////////////////////////////////
// Copyright 2003 DEIS - Universita' di Bologna
// 
// name         appsupport.h
// author       DEIS - Universita' di Bologna
//              Davide Bertozzi - dbertozzi@deis.unibo.it
//              Mirko Loghi - mloghi@deis.unibo.it
//              Federico Angiolini - fangiolini@deis.unibo.it
//              Francesco Poletti - fpoletti@deis.unibo.it
// portions by  Massimo Scardamaglia - mascard@vizzavi.it
// info         Provides support for testbench compilation
//
///////////////////////////////////////////////////////////////////////////////
*/

#ifndef __APPSUPPORT_H__
#define __APPSUPPORT_H__

#include "swi_calls.h"
#include "config.h"

#define start_metric()       __asm ("swi " SWI_METRIC_STARTstr)
#define stop_metric()        __asm ("swi " SWI_METRIC_STOPstr)
#define dump_metric()        __asm ("swi " SWI_METRIC_DUMPstr)
#define stop_simulation()    __asm ("swi " SWI_EXITstr)

extern int *semaphores;
extern unsigned int NODE_NUMBER;
extern unsigned int NNODES;

void pr(int proc, int msg_num, int num_arg);
unsigned int get_id();
unsigned int get_tot();

#endif // __APPSUPPORT_H__
