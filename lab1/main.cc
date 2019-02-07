#include <systemc.h>
#include "generator.h"
#include "sensor.h"

int sc_main(int argc, char **argv) {

  sc_set_default_time_unit(1, SC_SEC);
  sc_set_time_resolution(1, SC_MS);

  sc_signal<bool> N_o_sig, S_o_sig, E_o_sig, W_o_sig;

  Generator gen_north("North_traffic_generator");
  gen_north(N_o_sig);
  Generator gen_south("South_traffic_generator");
  gen_north(S_o_sig);
  Generator gen_west("West_traffic_generator");
  gen_north(W_o_sig);
  Generator gen_east("East_traffic_generator");
  gen_north(E_o_sig);

  /*
  gen_1(N_o_sig);
  Generator gen_2("South_traffic_generator");
  gen_2(S_o_sig);
  Generator gen_3("East_traffic_generator");
  gen_3(E_o_sig);
  Generator gen_4("West_traffic_generator");
  gen_4(W_o_sig);
  */

  Sensor sensor_north("North_traffic_Sensor");
  sensor_north(N_o_sig);
  Sensor sensor_south("South_traffic_Sensor");
  sensor_north(S_o_sig);
  Sensor sensor_west("West_traffic_Sensor");
  sensor_north(W_o_sig);
  Sensor sensor_east("East_traffic_Sensor");
  sensor_north(E_o_sig);



  sc_start(300, SC_SEC);
  return 0;
}
