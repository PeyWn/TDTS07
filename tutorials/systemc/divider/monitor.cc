#include <cassert>
#include "monitor.h"

Monitor::Monitor(sc_module_name name, char *outfile)
  : sc_module(name)
{
  assert(outfile != 0);
  out = new ofstream(outfile);
  assert(*out);

  SC_METHOD(monitor_method);
  dont_initialize();
  sensitive << quotient;

  SC_METHOD(check_constraints_method);
  dont_initialize();
  sensitive << denominator;
}

Monitor::~Monitor()
{
  delete out;
}

void Monitor::monitor_method()
{
  double q = quotient->read();
  *out << "quotient(" << sc_time_stamp() << ") = " << q << endl;
}

void Monitor::check_constraints_method()
{
  assert(denominator != 0);
}
