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
    wait(100, SC_MS);
    //print_event.notify();
    switch (currentState) {
      case ControllerState::WEST:
        W_o_p->write(true);

        if (E_i_p->read())
          currentState = ControllerState::EAST_AND_WEST;
        else if (N_i_p->read()) {
          wait(6, SC_SEC);
          W_o_p->write(false);
          currentState = ControllerState::NORTH;
        }
        else if (S_i_p->read()) {
          wait(6, SC_SEC);
          W_o_p->write(false);
          currentState = ControllerState::SOUTH;
        }
        else if (!W_i_p->read()) {
          W_o_p->write(false);
          currentState = ControllerState::NONE;
        }
        break;


      case ControllerState::EAST:
        E_o_p->write(true);

        if (W_i_p->read())
          currentState = ControllerState::EAST_AND_WEST;
        else if (N_i_p->read()) {
          wait(6, SC_SEC);
          E_o_p->write(false);
          currentState = ControllerState::NORTH;
        }
        else if (S_i_p->read()) {
          wait(6, SC_SEC);
          E_o_p->write(false);
          currentState = ControllerState::SOUTH;
        }
        else if (!E_i_p->read()) {
          E_o_p->write(false);
          currentState = ControllerState::NONE;
        }
        break;


      case ControllerState::EAST_AND_WEST:
        E_o_p->write(true);
        W_o_p->write(true);

        if (N_i_p->read()) {
          wait(6, SC_SEC);
          W_o_p->write(false);
          E_o_p->write(false);
          currentState = ControllerState::NORTH;
        }
        else if (S_i_p->read()) {
          wait(6, SC_SEC);
          W_o_p->write(false);
          E_o_p->write(false);
          currentState = ControllerState::SOUTH;
        }
        else if (!E_i_p->read() && W_i_p->read()) {
          E_o_p->write(false);
          currentState = ControllerState::WEST;
        }
        else if (!W_i_p->read() && E_i_p->read()) {
          W_o_p->write(false);
          currentState = ControllerState::EAST;
        }
        else if (!W_i_p->read() && !E_i_p->read()) {
          W_o_p->write(false);
          E_o_p->write(false);
          currentState = ControllerState::NONE;
        }
        break;


      case ControllerState::SOUTH:
        S_o_p->write(true);

        if (N_i_p->read())
          currentState = ControllerState::NORTH_AND_SOUTH;
        else if (E_i_p->read()) {
          wait(6, SC_SEC);
          S_o_p->write(false);
          currentState = ControllerState::EAST;
        }
        else if (W_i_p->read()) {
          wait(6, SC_SEC);
          S_o_p->write(false);
          currentState = ControllerState::WEST;
        }
        else if (!S_i_p->read()) {
          S_o_p->write(false);
          currentState = ControllerState::NONE;
        }
        break;

      case ControllerState::NORTH:
        N_o_p->write(true);

        if (S_i_p->read())
          currentState = ControllerState::NORTH_AND_SOUTH;
        else if (E_i_p->read()) {
          wait(6, SC_SEC);
          N_o_p->write(false);
          currentState = ControllerState::EAST;
        }
        else if (W_i_p->read()) {
          wait(6, SC_SEC);
          N_o_p->write(false);
          currentState = ControllerState::WEST;
        }
        else if (!N_i_p->read()) {
          N_o_p->write(false);
          currentState = ControllerState::NONE;
        }
        break;


      case ControllerState::NORTH_AND_SOUTH:
        N_o_p->write(true);
        S_o_p->write(true);

        if (E_i_p->read()) {
          wait(6, SC_SEC);
          N_o_p->write(false);
          S_o_p->write(false);
          currentState = ControllerState::EAST;
        }
        else if (W_i_p->read()) {
          wait(6, SC_SEC);
          N_o_p->write(false);
          S_o_p->write(false);
          currentState = ControllerState::WEST;
        }
        else if (!N_i_p->read() && S_i_p->read()) {
          N_o_p->write(false);
          currentState = ControllerState::SOUTH;
        }
        else if (!S_i_p->read() && N_i_p->read()) {
          S_o_p->write(false);
          currentState = ControllerState::NORTH;
        }
        else if (!N_i_p->read() && !S_i_p->read()) {
          N_o_p->write(false);
          S_o_p->write(false);
          currentState = ControllerState::NONE;
        }
        break;


      case ControllerState::NONE:
        if (N_i_p->read())
          currentState = ControllerState::NORTH;
        else if (S_i_p->read())
          currentState = ControllerState::SOUTH;
        else if (E_i_p->read())
          currentState = ControllerState::EAST;
        else if (W_i_p->read())
          currentState = ControllerState::WEST;
        break;
    }
  }
}

void Controller::print_method() {
  switch (currentState) {
    case ControllerState::WEST:
      cout << "   -   " << endl;
      cout << " *   - " << endl;
      cout << "   -   " << endl;
      cout << endl;
      break;
    case ControllerState::EAST:
      cout << "   -   " << endl;
      cout << " -   * " << endl;
      cout << "   -   " << endl;
      cout << endl;
      break;
    case ControllerState::EAST_AND_WEST:
      cout << "   -   " << endl;
      cout << " *   * " << endl;
      cout << "   -   " << endl;
      cout << endl;
      break;
    case ControllerState::SOUTH:
      cout << "   -   " << endl;
      cout << " -   - " << endl;
      cout << "   *   " << endl;
      cout << endl;
      break;
    case ControllerState::NORTH:
      cout << "   *   " << endl;
      cout << " -   - " << endl;
      cout << "   -   " << endl;
      cout << endl;
      break;
    case ControllerState::NORTH_AND_SOUTH:
      cout << "   *   " << endl;
      cout << " -   - " << endl;
      cout << "   *   " << endl;
      cout << endl;
      break;
    case ControllerState::NONE:
      cout << "   -   " << endl;
      cout << " -   - " << endl;
      cout << "   -   " << endl;
      cout << endl;
      break;
  }
}
