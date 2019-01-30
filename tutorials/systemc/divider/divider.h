#ifndef DIVIDER_H
#define DIVIDER_H

#include <systemc.h>

SC_MODULE(Divider) {
  sc_in<int> numerator;
  sc_in<int> denominator;
  sc_out<double> quotient;

  SC_HAS_PROCESS(Divider);
  Divider(sc_module_name name);

  void divide_method();
};

#endif // DIVIDER_H
