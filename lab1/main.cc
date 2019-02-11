#include <systemc.h>
#include "generator.h"
#include "controller.h"
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

  // N<orth>G<enerator>_Sig<nal>
  sc_signal<bool> NG_sig, SG_sig, EG_sig, WG_sig;
  // N<orth>T<raffic>L<light>_Sig<nal>
  sc_signal<bool> NTL_sig, STL_sig, ETL_sig, WTL_sig;
  // N<orth>S<ensor>_Sig<nal>
  sc_signal<bool> NS_sig, SS_sig, ES_sig, WS_sig;

  Generator gen_north("North_traffic_generator", northfile);
  gen_north(NG_sig);
  Generator gen_south("South_traffic_generator", southfile);
  gen_south(SG_sig);
  Generator gen_west("West_traffic_generator ", westfile);
  gen_west(WG_sig);
  Generator gen_east("East_traffic_generator ", eastfile);
  gen_east(EG_sig);

  Sensor sensor_north("North_traffic_Sensor");
  sensor_north(NG_sig, NTL_sig, NS_sig);
  Sensor sensor_south("South_traffic_Sensor");
  sensor_south(SG_sig, STL_sig, SS_sig);
  Sensor sensor_west("West_traffic_Sensor");
  sensor_west(WG_sig, WTL_sig, WS_sig);
  Sensor sensor_east("East_traffic_Sensor");
  sensor_east(EG_sig, ETL_sig, ES_sig);

  Controller traffic_light("Traffic light");
  traffic_light(NS_sig, SS_sig, ES_sig, WS_sig, NTL_sig, STL_sig, ETL_sig, WTL_sig);

  sc_start(sim_time);
  return 0;
}
