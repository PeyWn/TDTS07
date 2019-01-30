#ifndef INPUT_GEN_H
#define INPUT_GEN_H

#include <systemc.h>
#include <fstream>

using std::ifstream;

SC_MODULE(Generator) {
  sc_out<int> numerator;
  sc_out<int> denominator;

  SC_HAS_PROCESS(Generator);
  Generator(sc_module_name name, char *datafile);
  ~Generator();

  void generate_thread();

  ifstream *in;
};

#endif // INPUT_GEN_H
