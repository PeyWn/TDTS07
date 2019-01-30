#include "input_gen.h"
#include <cassert>

Generator::Generator(sc_module_name name, char *datafile)
  : sc_module(name)
{
  assert(datafile != 0);       // An input file should be given.

  in = new ifstream(datafile); // Open the input file.
  assert(*in);                 // Check that everything is OK.

  SC_THREAD(generate_thread);

  numerator.initialize(0);
  denominator.initialize(0);
}

Generator::~Generator()
{
  delete in;
}

void Generator::generate_thread()
{
  int num, denom;
  for (;;) {
    wait(1, SC_SEC);     // Generate new inputs every second.
    *in >> num >> denom; // Read from the input file.
    numerator->write(num);
    denominator->write(denom);
  }
}
