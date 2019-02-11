#include "sensor.h"
using std::cout;
using std::endl;

Sensor::Sensor(sc_module_name name)
  : sc_module(name)
{
  S_o_p.initialize(0);

  SC_THREAD(sensor_thread);
  dont_initialize();
  sensitive << G_i_p << TL_i_p;

  SC_METHOD(print_method);
  dont_initialize();
  sensitive << print_ev;

}

void Sensor::print_method() {
  const char* module_name = sc_core::sc_get_current_process_b()->get_parent()->basename();
  cout << sc_time_stamp() << ": Cars in sensor " << module_name << ": " << cars << endl;
}

void Sensor::sensor_thread() {
  cars = 0;
  for (;;) {
    wait(8, SC_SEC);

    if (!cars == 0)
      S_o_p->write(true);
    else
      S_o_p->write(false);

    if (G_i_p->read()) {
      cars++;
      print_ev.notify();
    }

    if (TL_i_p->read() && !cars == 0) {
      wait(2, SC_SEC);
      cars--;
      print_ev.notify();
    }

  }
}
