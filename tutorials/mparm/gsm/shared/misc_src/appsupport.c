/*
///////////////////////////////////////////////////////////////////////////////
// Copyright 2003 DEIS - Universita' di Bologna
//
// name         appsupport.c
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

#include "appsupport.h"

int *semaphores    =    (int *)0x20000000;
unsigned int NODE_NUMBER;
unsigned int NNODES;

///////////////////////////////////////////////////////////////////////////////
// pr - Allows printing debug info even without support from an OS. See
//      user_swi.cpp for printable messages, or to create your own
void pr(int proc, int msg_num, int num_arg)
{
#ifndef __OPTIMIZE__
    __asm ("ldr r1, %0" : : "g" (proc) : "r1" );
    __asm ("ldr r2, %0" : : "g" (msg_num) : "r2" );
    __asm ("ldr r3, %0" : : "g" (num_arg) : "r3" );
    __asm ("swi " SWI_PRINTstr);
#else
    __asm ("mov r1, %0" : : "g" (proc) : "r1" );
    __asm ("mov r2, %0" : : "g" (msg_num) : "r2" );
    __asm ("mov r3, %0" : : "g" (num_arg) : "r3" );
    __asm ("swi " SWI_PRINTstr);
#endif
}

///////////////////////////////////////////////////////////////////////////////
// get_id - Allows getting the processor's ID (from 1 onwards). It is based
//          upon coprocessor registers
unsigned int get_id()
{
  unsigned int __t;
  __asm__ volatile ("mrc 15, 0, %0, c11, c0, 3" : "=r" (__t) :);
  return __t+1;
}

///////////////////////////////////////////////////////////////////////////////
// get_tot - Allows getting the number of processors in the platform. It is
//           based upon coprocessor registers
unsigned int get_tot()
{
  unsigned int __t;
  __asm__ volatile ("mrc 15, 0, %0, c11, c0, 4" : "=r" (__t) :);
  return __t;
}
