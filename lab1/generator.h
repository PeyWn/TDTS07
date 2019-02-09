#ifndef GENERATOR_H
#define GENERATOR_H

#include <systemc.h>
#include <iostream>
#include <fstream>

using std::ifstream;
int nb_car = 100;

SC_MODULE(Generator) {
  sc_out<bool> o_p;
  int C_queue [100];
  sc_event print_event;

  SC_HAS_PROCESS(Generator);
  Generator(sc_module_name name, char *datafile);
  ~Generator();

  void gen_thread();
  void print_method();

  ifstream *in;

};

#endif
