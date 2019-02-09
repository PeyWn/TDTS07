#include "generator.h"
#include <cassert>

using std::cout;
using std::endl;

int nb_car = 100;

Generator::Generator(sc_module_name name, char *datafile)
  : sc_module(name)
{
  assert(datafile != 0);       // An input file should be given.

  in = new ifstream(datafile); // Open the input file.
  assert(*in);                 // Check that everything is OK.
  
  SC_THREAD(gen_thread);
  SC_METHOD(print_method);

  o_p.initialize(0);
}

void Generator::print_method() {
  cout << sc_time_stamp() << ": Cars " << o_p << endl;

}

Generator::~Generator()
{
  delete in;
}

void Generator::gen_thread() {
  int i = 0;
  for (;;) {
    wait(8, SC_SEC);
    *in >> C_queue[i];
    if ( ++i < nb_car ) {
      o_p->write(C_queue[i]);
    }
    else{
      i = 0;
      }
  }
}
