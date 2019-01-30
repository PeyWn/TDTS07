#include <systemc.h>
#include <iostream>
#include <time.h>

using std::cout;
using std::endl;

int N_queue[100];

void gen_random_list(int *array, int n) {
  for (int i = 0; i <= n; i++)
    array[i] = rand()%2;
}

SC_MODULE(Generator) {
  sc_out<int> o_p;

  void gen_thread() {
    for (;;) {
      wait(2, SC_SEC);
      if (sizeof(N_queue) != 0)
        o_p->write(N_queue.pop());
    }
  }
  void print_method() {
    cout << sc_time_stamp() << ": Car " << o_p << endl;
  }

  SC_CTOR(Generator) {
    o_p.initialize(0);
    SC_THREAD(gen_thread);
    SC_METHOD(print_method);
    sensitive << o_p;
  }
}; // END Generator



int sc_main(int argc, char *argv[]) {
  /*
  sc_set_default_time_unit(1, SC_SEC);
  sc_set_time_resolution(1, SC_MS);

  */
  srand(time(NULL));
  sc_signal<int> o_sig;
  gen_random_list(N_queue, sizeof(N_queue));
  Generator gen("Generator_1");
  gen(o_sig);

  sc_start(300, SC_SEC);
  return 0;
}
