#ifndef GENERATOR_H
#define GENERATOR_H

#include <systemc.h>
#include <iostream>
#include <time.h>

SC_MODULE(Generator) {
  sc_out<bool> o_p;
  int C_queue [100];

  SC_HAS_PROCESS(Generator);
  Generator(sc_module_name name);
  void gen_thread();
  void print_method();

};

#endif
