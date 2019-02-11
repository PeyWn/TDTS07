#ifndef SENSOR_H
#define SENSOR_H

#include <systemc.h>
#include <iostream>

SC_MODULE(Sensor) {
  sc_in<bool> G_i_p, TL_i_p; // i_p --> Input port
  sc_out<bool> S_o_p;
  sc_event print_ev;
  int cars;

  SC_HAS_PROCESS(Sensor);
  Sensor(sc_module_name name);
  void print_method();
  void sensor_thread();

};

#endif
