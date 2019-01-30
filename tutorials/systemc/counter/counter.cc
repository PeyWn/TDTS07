#include <iostream>
#include "counter.h"

using std::cout;
using std::endl;

Counter::Counter(sc_module_name name, int start)
  : sc_module(name), value(start)
{
  SC_THREAD(event_trigger_thread);

  SC_METHOD(count_method);
  dont_initialize();
  sensitive << count_event;
}

void Counter::count_method()
{
  value++;
  cout << sc_time_stamp() << ": Counter has value "
       << value << "." << endl;
}

void Counter::event_trigger_thread()
{
  for (;;) {               // Loop infinitely.
    wait(1, SC_SEC);       // Wait one second.
    count_event.notify();  // Trigger count_method.
  }
}
