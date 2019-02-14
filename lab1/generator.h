#ifndef GENERATOR_H
#define GENERATOR_H

#include <systemc.h>
#include <iostream>
#include <fstream>

using std::ifstream;

SC_MODULE(Generator) {
  sc_out<bool> o_p;
  sc_event print_event;
  sc_event gen_event;
  std::string line;

  unsigned long i;
  int nb_car;
  int car;
  bool toggle;

  SC_HAS_PROCESS(Generator);
  Generator(sc_module_name name, char *datafile);
  ~Generator();

  void gen_method();
  void print_method();
  void event_creator();

  ifstream *in;

};

#endif
