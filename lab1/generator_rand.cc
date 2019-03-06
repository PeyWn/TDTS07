#include "generator_rand.h"
#include <cassert>

using std::cout;
using std::endl;

Generator::Generator(sc_module_name name)
  : sc_module(name)
{

  toggle = true;
  srand(time(NULL)); //Init rand

  SC_THREAD(event_creator);
  SC_METHOD(gen_method);
  sensitive << gen_event;
  SC_METHOD(print_method);
  dont_initialize();
  sensitive << print_event;
  o_p.initialize(0);
}

void Generator::print_method() {
  const char* module_name = sc_core::sc_get_current_process_b()->get_parent()->basename();
  cout << sc_time_stamp() << ": Cars for " << module_name << ": " << o_p->read() << endl;

}

Generator::~Generator()
{
}

void Generator::gen_method() {
  car = rand()%14; //car <= 4 30% chance of generating a car
  if (car <= 4) {
    if(toggle) {
      o_p->write(true);
      toggle = !toggle;
    }
    else {
      o_p->write(false);
      toggle = !toggle;
    }
  }
}

void Generator::event_creator() {
  for(;;) {
    wait(2, SC_SEC);
    gen_event.notify();
    //wait(0, SC_SEC);
    //print_event.notify();
  }
}
