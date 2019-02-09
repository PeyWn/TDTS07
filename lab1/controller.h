#ifndef CONTRLLER_H
#define CONTRLLER_H

#include <systemc.h>

struct ControllerState{
  enum Traffic {WEST_BOUND, EAST_BOUND, EAST_AND_WEST_BOUND,
    SOUTH_BOUND, NORTH_BOUND, NORTH_AND_SOUTH_BOUND, NONE};
};

SC_MODULE(Controller) {
  sc_in<bool> N_i_p, S_i_p, E_i_p, W_i_p;
  sc_out<bool> N_o_p, S_o_p, E_o_p, W_o_p;

  SC_HAS_PROCESS(Controller);
  Controller(sc_module_name name);

  void controller_method();
}

#endif
