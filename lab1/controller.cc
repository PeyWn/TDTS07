#include <controller.h>

using std::cout;
using std::endl;

ControllerState::Traffic currentState = ControllerState::NONE;

Controller::Controller(sc_module_name name)
  : sc_module(name)
{
  N_o_p.initialize(0);
  E_o_p.initialize(0);
  S_o_p.initialize(0);
  W_o_p.initialize(0);


  SC_METHOD(print_method);
  sensitive << print_event;
  SC_THREAD(controller_thread);

}

void Controller::controller_thread() {

  for(;;) {
    wait(4, SC_SEC);
    print_event.notify();
    switch (currentState) {
      case ControllerState::WEST_BOUND:
        W_o_p->write(true);

        if (E_i_p->read())
          currentState = ControllerState::EAST_AND_WEST_BOUND;
        else if (N_i_p->read()) {
          wait(2, SC_SEC);
          W_o_p->write(false);
          currentState = ControllerState::NORTH_BOUND;
        }
        else if (S_i_p->read()) {
          wait(2, SC_SEC);
          W_o_p->write(false);
          currentState = ControllerState::SOUTH_BOUND;
        }
        else if (!W_i_p->read()) {
          W_o_p->write(false);
          currentState = ControllerState::NONE;
        }
        break;


      case ControllerState::EAST_BOUND:
        E_o_p->write(true);

        if (W_i_p->read())
          currentState = ControllerState::EAST_AND_WEST_BOUND;
        else if (N_i_p->read()) {
          wait(2, SC_SEC);
          E_o_p->write(false);
          currentState = ControllerState::NORTH_BOUND;
        }
        else if (S_i_p->read()) {
          wait(2, SC_SEC);
          E_o_p->write(false);
          currentState = ControllerState::SOUTH_BOUND;
        }
        else if (!E_i_p->read()) {
          E_o_p->write(false);
          currentState = ControllerState::NONE;
        }
        break;


      case ControllerState::EAST_AND_WEST_BOUND:
        E_o_p->write(true);
        W_o_p->write(true);

        if (N_i_p->read()) {
          wait(2, SC_SEC);
          W_o_p->write(false);
          E_o_p->write(false);
          currentState = ControllerState::NORTH_BOUND;
        }
        else if (S_i_p->read()) {
          wait(2, SC_SEC);
          W_o_p->write(false);
          E_o_p->write(false);
          currentState = ControllerState::SOUTH_BOUND;
        }
        else if (!E_o_p->read() && W_o_p->read()) {
          E_o_p->write(false);
          currentState = ControllerState::WEST_BOUND;
        }
        else if (!W_o_p->read() && E_o_p->read()) {
          W_o_p->write(false);
          currentState = ControllerState::EAST_BOUND;
        }
        else if (!W_o_p->read() && !E_o_p->read()) {
          W_o_p->write(false);
          E_o_p->write(false);
          currentState = ControllerState::NONE;
        }
        break;


      case ControllerState::SOUTH_BOUND:
        S_o_p->write(true);

        if (N_i_p->read())
          currentState = ControllerState::NORTH_AND_SOUTH_BOUND;
        else if (E_i_p->read()) {
          wait(2, SC_SEC);
          S_o_p->write(false);
          currentState = ControllerState::EAST_BOUND;
        }
        else if (W_i_p->read()) {
          wait(2, SC_SEC);
          S_o_p->write(false);
          currentState = ControllerState::WEST_BOUND;
        }
        else if (!S_i_p->read()) {
          S_o_p->write(false);
          currentState = ControllerState::NONE;
        }
        break;

      case ControllerState::NORTH_BOUND:
        N_o_p->write(true);

        if (S_i_p->read())
          currentState = ControllerState::NORTH_AND_SOUTH_BOUND;
        else if (E_i_p->read()) {
          wait(2, SC_SEC);
          N_o_p->write(false);
          currentState = ControllerState::EAST_BOUND;
        }
        else if (W_i_p->read()) {
          wait(2, SC_SEC);
          N_o_p->write(false);
          currentState = ControllerState::WEST_BOUND;
        }
        else if (!N_i_p->read()) {
          N_o_p->write(false);
          currentState = ControllerState::NONE;
        }
        break;


      case ControllerState::NORTH_AND_SOUTH_BOUND:
        N_o_p->write(true);
        S_o_p->write(true);

        if (E_i_p->read()) {
          wait(2, SC_SEC);
          N_o_p->write(false);
          S_o_p->write(false);
          currentState = ControllerState::EAST_BOUND;
        }
        else if (W_i_p->read()) {
          wait(2, SC_SEC);
          N_o_p->write(false);
          S_o_p->write(false);
          currentState = ControllerState::WEST_BOUND;
        }
        else if (!N_o_p->read() && S_o_p->read()) {
          N_o_p->write(false);
          currentState = ControllerState::SOUTH_BOUND;
        }
        else if (!S_o_p->read() && N_o_p->read()) {
          S_o_p->write(false);
          currentState = ControllerState::NORTH_BOUND;
        }
        else if (!N_o_p->read() && !S_o_p->read()) {
          N_o_p->write(false);
          S_o_p->write(false);
          currentState = ControllerState::NONE;
        }
        break;


      case ControllerState::NONE:
        if (N_i_p->read())
          currentState = ControllerState::NORTH_BOUND;
        else if (S_i_p->read())
          currentState = ControllerState::SOUTH_BOUND;
        else if (E_i_p->read())
          currentState = ControllerState::EAST_BOUND;
        else if (W_i_p->read())
          currentState = ControllerState::WEST_BOUND;
        break;
    }
  }
}

void Controller::print_method() {
  switch (currentState) {
    case ControllerState::WEST_BOUND:
      cout << "   -   " << endl;
      cout << " *   - " << endl;
      cout << "   _   " << endl;
      cout << endl;
      break;
    case ControllerState::EAST_BOUND:
      cout << "   -   " << endl;
      cout << " -   * " << endl;
      cout << "   _   " << endl;
      cout << endl;
      break;
    case ControllerState::EAST_AND_WEST_BOUND:
      cout << "   -   " << endl;
      cout << " *   * " << endl;
      cout << "   _   " << endl;
      cout << endl;
      break;
    case ControllerState::SOUTH_BOUND:
      cout << "   -   " << endl;
      cout << " -   - " << endl;
      cout << "   *   " << endl;
      cout << endl;
      break;
    case ControllerState::NORTH_BOUND:
      cout << "   *   " << endl;
      cout << " -   - " << endl;
      cout << "   _   " << endl;
      cout << endl;
      break;
    case ControllerState::NORTH_AND_SOUTH_BOUND:
      cout << "   *   " << endl;
      cout << " -   - " << endl;
      cout << "   *   " << endl;
      cout << endl;
      break;
    case ControllerState::NONE:
      cout << "   -   " << endl;
      cout << " -   - " << endl;
      cout << "   _   " << endl;
      cout << endl;
      break;
  }
}
