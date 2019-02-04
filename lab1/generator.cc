#include <systemc.h>
#include <iostream>
#include <time.h>

using std::cout;
using std::endl;


int nb_car = 100;
int N_queue[100];
int S_queue[100];
int E_queue[100];
int W_queue[100];


void gen_random_list(int *array, int n) {
  for (int i = 0; i <= n; i++)
    array[i] = rand()%2;
}

SC_MODULE(Generator) {
  sc_out<bool> o_p{"gen_out"};
  int C_queue [100];

  void gen_thread() {
    int i = 0;
    for (;;) {
      wait(2, SC_SEC);
      if ( ++i < nb_car ) {
        o_p->write(C_queue[i]);
      }
      else{
        gen_random_list(C_queue, nb_car);
        i = 0;
        }
    }
  }

  void print_method() {
    cout << sc_time_stamp() << ": Cars " << o_p << endl;
  }

  SC_CTOR(Generator) {
    o_p.initialize(0);
    gen_random_list(C_queue, nb_car);

    SC_THREAD(gen_thread);
    SC_METHOD(print_method);
    sensitive << o_p;
  }
}; // END Generator


SC_MODULE(Sensor) {
  sc_in<bool> N_i_p, S_i_p, E_i_p, W_i_p;
  sc_out<bool> NS_o_p, SS_o_p, ES_o_p, WS_o_p;
  sc_event print_ev;

  void sensor_method() {
    for (;;) {
      if (N_i_p)
        NS_o_p = true;
      else
        NS_o_p = false;
      if (S_i_p)
        SS_o_p = true;
      else
        SS_o_p = false;
      if (E_i_p)
        ES_o_p = true;
      else
        ES_o_p = false;
      if (W_i_p)
        WS_o_p = true;
      else
        WS_o_p = false;
    }
  }

  void print_method() {

  }

  SC_CTOR(Sensor) {
    NS_o_p.initialize(0);
    SS_o_p.initialize(0);
    ES_o_p.initialize(0);
    WS_o_p.initialize(0);

    SC_METHOD(sensor_method);
    sensitive << N_i_p << S_i_p << E_i_p << W_i_p;

    SC_METHOD(print_method);
    dont_initialize();
    sensitive << print_ev;

  }
}; // END Sensor


int sc_main(int argc, char *argv[]) {


  srand(time(NULL));

  sc_set_default_time_unit(1, SC_SEC);
  sc_set_time_resolution(1, SC_MS);

  sc_signal<bool> N_o_sig, S_o_sig, E_o_sig, W_o_sig;
  Generator gen_1("North_traffic_generator");
  gen_1(N_o_sig);
  /*
  gen_1(N_o_sig);
  Generator gen_2("South_traffic_generator");
  gen_2(S_o_sig);
  Generator gen_3("East_traffic_generator");
  gen_3(E_o_sig);
  Generator gen_4("West_traffic_generator");
  gen_4(W_o_sig);
  */

  /*
  Sensor sensor_module("Sensor_1");
  sensor_module(N_o_sig, S_o_sig, E_o_sig, W_o_sig);
  */

  sc_start(300, SC_SEC);
  return 0;
}
