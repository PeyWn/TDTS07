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

  i = 0;
  toggle = true;
  //srand(time(NULL)); //Init rand

  getline(*in, line);
  nb_car = atoi(line.c_str());
  getline(*in, line);
  in->close();

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
  delete in;
}

void Generator::gen_method() {
  if ( i < nb_car) {
    car = (int)line[i]%48; //rand()%14 then car <= 4 30% chance of generating a car
    if (car == 1) {
      if(toggle) {
        o_p->write(true);
        toggle = !toggle;
      }
      else {
        o_p->write(false);
        toggle = !toggle;
      }
    }
    i++;
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
