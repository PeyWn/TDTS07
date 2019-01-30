/*  system.h
 *
 *  This include file contains information that is included in every
 *  function in the test set.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  system.h,v 1.12 2000/06/12 15:00:11 joel Exp
 */

//#include "tmacros.h"
#include "appsupport.h"

// Functions

// Configuration information
#define CONFIGURE_TEST_NEEDS_CONSOLE_DRIVER

//Task scheduling working
#define CONFIGURE_TEST_NEEDS_CLOCK_DRIVER

//Tell the os to swhitch a thread every 1ms if when in timeslicing policy 
#define CONFIGURE_TICKS_PER_TIMESLICE 1

// Tells confdefs.h to build the init task configuration
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_MAXIMUM_TASKS 10

// Tells confdefs.h to build the default configuration table
#define CONFIGURE_INIT

//#include <confdefs.h>

