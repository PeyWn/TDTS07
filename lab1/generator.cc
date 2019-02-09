#include "generator.h"
#include <cassert>

using std::cout;
using std::endl;

Generator::Generator(sc_module_name name, char *datafile)
  : sc_module(name)
{
  assert(datafile != 0);       // An input file should be given.

  in = new ifstream(datafile); // Open the input file.
  assert(*in);                 // Check that everything is OK.

  SC_THREAD(gen_thread);
  SC_METHOD(print_method);
  sensitive << print_event;
  o_p.initialize(0);
}

void Generator::print_method() {
  const char* module_name = sc_core::sc_get_current_process_b()->get_parent()->basename();
  cout << sc_time_stamp() << ": Cars for " << module_name << ": " << o_p->read() << endl;

}

Generator::~Generator()
{
  delete in;
}

void Generator::gen_thread() {
  int i = 0;
  for (;;) {
    wait(8, SC_SEC);
    print_event.notify();
    *in >> C_queue[i];
    if ( i < nb_car ) {
      o_p->write(C_queue[i]);
      i++;
    }
    else{
      i = 0;
      }
  }
}
