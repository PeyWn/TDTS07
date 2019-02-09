#ifndef CONTRLLER_H
#define CONTRLLER_H

SC_MODULE(Controller) {
  sc_in<bool> N_i_p, S_i_p, E_i_p, W_i_p;
  sc_out<bool> N_o_p, S_o_p, E_o_p, W_o_p;
}

#endif
