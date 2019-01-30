#include <cassert>
#include <systemc.h>
#include "counter.h"

int sc_main(int argc, char **argv)
{
  // The command-line arguments are as follows:
  // 1. the initial value of the counter and
  // 2. the simulation time (in seconds).
  assert(argc == 3);

  int init_value = atoi(argv[1]);
  sc_time sim_time(atof(argv[2]), SC_SEC);

  // Instantiate the module.
  Counter c1("Counter_1", init_value);

  // Start the simulation.
  sc_start(sim_time);

  return 0;
}
