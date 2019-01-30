#include <systemc.h>
#include <iostream>

using std::cout;
using std::endl;

SC_MODULE(Adder) {
  sc_in<int> a_p;
  sc_in<int> b_p;
  sc_out<int> sum_p;
  sc_event print_ev;

  void add_method() {
    sum_p = a_p + b_p;
    print_ev.notify(SC_ZERO_TIME);
  }

  void print_method() {
    cout << sc_time_stamp() << ": Sum=" << sum_p << endl;
  }

  SC_CTOR(Adder) {
    sum_p.initialize(0);

    SC_METHOD(add_method);
    sensitive << a_p << b_p;

    SC_METHOD(print_method);
    dont_initialize();
    sensitive << print_ev;
  }
}; // END Adder

SC_MODULE(Generator) {
  sc_out<int> a_p;
  sc_out<int> b_p;

  void gen_thread() {
    for (;;) {
      wait(1, SC_SEC);
      a_p = a_p + 1;
      b_p->write(b_p->read() + 1);
    }
  }

  SC_CTOR(Generator) {
    a_p.initialize(0);
    b_p.initialize(0);
    SC_THREAD(gen_thread);
  }
}; // END Generator

int sc_main(int argc, char *argv[]) {
  sc_set_default_time_unit(1, SC_SEC);
  sc_set_time_resolution(1, SC_MS);

  sc_signal<int> a_sig, b_sig, sum_sig;

  Adder adder_module("Adder_1");
  adder_module(a_sig, b_sig, sum_sig);

  Generator gen("Generator_1");
  gen(a_sig, b_sig);

  sc_start(30, SC_SEC);
  return 0;
}
