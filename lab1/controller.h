#ifndef CONTRLLER_H
#define CONTRLLER_H

#include <systemc.h>

struct ControllerState{
  enum Traffic {WEST, EAST, EAST_AND_WEST,
    SOUTH, NORTH, NORTH_AND_SOUTH, NONE};
};

SC_MODULE(Controller) {
  sc_in<bool> N_i_p, S_i_p, E_i_p, W_i_p;
  sc_out<bool> N_o_p, S_o_p, E_o_p, W_o_p;
  sc_event print_event;

  SC_HAS_PROCESS(Controller);
  Controller(sc_module_name name);

  void controller_thread();
  void print_method();
};

#endif
