#ifndef MONITOR_H
#define MONITOR_H

#include <systemc.h>
#include <fstream>

using std::ofstream;

SC_MODULE(Monitor) {
  sc_in<double> quotient;
  sc_in<int> denominator;

  SC_HAS_PROCESS(Monitor);
  Monitor(sc_module_name name, char *outfile);
  ~Monitor();

  void monitor_method();
  void check_constraints_method();

  ofstream *out;
};

#endif // MONITOR_H
