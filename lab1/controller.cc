#include <controller.h>

using std::cout;
using std::endl;

typedef struct {
  enum  {WEST_BOUND, EAST_BOUND, EAST_AND_WEST_BOUND,
     SOUTH_BOUND, NORTH_BOUND, NORTH_AND_SOUTH_BOUND, NONE};
}ControllerState;


Controller::Controller(sc_module_name name)
  : sc_module(name)
{
  N_o_p.initialize(0);
  E_o_p.initialize(0);
  S_o_p.initialize(0);
  W_o_p.initialize(0);

  ControllerState currentState = ControllerState::NONE;
  SC_METHOD(controller_method);
  sensitive << N_i_p, S_i_p, E_i_p, W_i_p;

}

void Controller::controller_method() {

  for(;;) {
    switch (currentState) {
      case ControllerState::WEST_BOUND:
        cout << "ControllerState West" << endl;
        break;
      case ControllerState::EAST_BOUND:
        cout << "ControllerState East" << endl;
        break;
      case ControllerState::EAST_AND_WEST_BOUND;
        cout << "ControllerState East and West" << endl;
        break;
      case ControllerState::SOUTH_BOUND:
        cout << "ControllerState South" << endl;
        break;
      case ControllerState::NORTH_BOUND:
        cout << "ControllerState North" << endl;
        break;
      case ControllerState::NORTH_AND_SOUTH_BOUND;
        cout << "ControllerState North and South" << endl;
        break;
      case ControllerState::NONE
        cout << "ControllerState None" << endl;
        break;
    }
  }
}

void Controller::print_method() {
  cout << '' << currentState << endl;
}
