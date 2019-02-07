#include "generator.h"

using std::cout;
using std::endl;

int nb_car = 100;

void gen_random_list(int *array, int n) {
  srand(time(NULL));
  for (int i = 0; i <= n; i++)
    array[i] = rand()%2;
}

Generator::Generator(sc_module_name name)
  : sc_module(name)
{
  o_p.initialize(0);
  gen_random_list(C_queue, nb_car);

  SC_THREAD(gen_thread);
  SC_METHOD(print_method);
  sensitive << o_p;
}

void Generator::print_method() {
  cout << sc_time_stamp() << ": Cars " << o_p << endl;

}

void Generator::gen_thread() {
  int i = 0;
  for (;;) {
    wait(8, SC_SEC);
    if ( ++i < nb_car ) {
      o_p->write(C_queue[i]);
    }
    else{
      gen_random_list(C_queue, nb_car);
      i = 0;
      }
  }
}
