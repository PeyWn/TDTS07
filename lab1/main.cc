#include <systemc.h>
#include "generator.h"
#include "sensor.h"

int sc_main(int argc, char **argv) {

  /*
  sc_set_default_time_unit(1, SC_SEC);
  sc_set_time_resolution(1, SC_MS);
  */
  assert(argc == 7);

  sc_time sim_time(atof(argv[1]), SC_SEC);
  char *northfile = argv[2];
  char *southfile = argv[3];
  char *westfile = argv[4];
  char *eastfile = argv[5];
  char *outfile = argv[6];

  sc_signal<bool> N_o_sig, S_o_sig, E_o_sig, W_o_sig;

  Generator gen_north("North_traffic_generator", northfile);
  gen_north(N_o_sig);

  Generator gen_south("South_traffic_generator", southfile);
  gen_south(S_o_sig);

  Generator gen_west("West_traffic_generator ", westfile);
  gen_west(W_o_sig);

  Generator gen_east("East_traffic_generator ", eastfile);
  gen_east(E_o_sig);


  /*
  Sensor sensor_north("North_traffic_Sensor");
  sensor_north(N_o_sig);
  Sensor sensor_south("South_traffic_Sensor");
  sensor_north(S_o_sig);
  Sensor sensor_west("West_traffic_Sensor");
  sensor_north(W_o_sig);
  Sensor sensor_east("East_traffic_Sensor");
  sensor_north(E_o_sig);
  */


  sc_start(sim_time);
  return 0;
}
