#include "sensor.h"
int cars = 0;

Sensor::Sensor(sc_module_name name)
  : sc_module(name)
{
  S_o_p.initialize(0);


  SC_THREAD(sensor_thread);
  sensitive << C_i_p << TL_i_p;

  SC_METHOD(print_method);
  dont_initialize();
  sensitive << print_ev;

}

void Sensor::print_method() {

}

void Sensor::sensor_thread() {

  for (;;) {

    if (C_i_p->read())
      cars++;
    if (TL_i_p->read())
      //TODO one car per 2 seconds
      cars--;
    if (cars)
      S_o_p = true;
    else
      S_o_p = false;

  }
}
