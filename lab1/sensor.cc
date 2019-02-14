#include "sensor.h"
using std::cout;
using std::endl;

Sensor::Sensor(sc_module_name name)
  : sc_module(name)
{
  cars = 0;
  S_o_p.initialize(0);

  SC_METHOD(sensor_increment_method);
  dont_initialize();
  sensitive << G_i_p;

  SC_THREAD(sensor_decrement_thread);

  SC_METHOD(print_method);
  //dont_initialize();
  sensitive << print_ev;

}

void Sensor::print_method() {
  const char* module_name = sc_core::sc_get_current_process_b()->get_parent()->basename();
  cout << sc_time_stamp() << ": Cars in sensor " << module_name << ": " << cars << endl;
}

void Sensor::sensor_increment_method() {
  cars++;
  //const char* module_name = sc_core::sc_get_current_process_b()->get_parent()->basename();
  //cout << sc_time_stamp() << ": Cars in sensor incre " << module_name << ": " << cars << endl;
  print_ev.notify();
}

void Sensor::sensor_decrement_thread() {
  for(;;){
    wait(2, SC_SEC);
    if (!cars == 0)
      S_o_p->write(true);
    else
      S_o_p->write(false);

    if (!cars == 0 && TL_i_p->read()) {
      cars--;
      //const char* module_name = sc_core::sc_get_current_process_b()->get_parent()->basename();
      //cout << sc_time_stamp() << ": Cars in sensor decre " << module_name << ": " << cars << endl;
      print_ev.notify();
    }

  }

}
