#include "divider.h"

Divider::Divider(sc_module_name name)
  : sc_module(name)
{
  quotient.initialize(0);

  SC_METHOD(divide_method);
  dont_initialize();
  sensitive << numerator << denominator;
}

void Divider::divide_method()
{
  int num = numerator->read();
  int denom = denominator->read();
  double q = double(num) / denom;
  quotient->write(q);
}
