/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this program.  If not, see
<http://www.gnu.org/licenses/>.
*/

#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <fsmos.h>

#define BTN_PIN (2)

#define BTN_WAIT_DOWN (FSM_STATE_USERDEF + 1)
#define BTN_WAIT_UP (FSM_STATE_USERDEF + 2)

extern LedTask ledTask;

class ButtonTask : public FsmTask {
public:
  virtual void init() {
    pinMode(BTN_PIN, INPUT_PULLUP);
    this->gotoState(BTN_WAIT_DOWN);
  };

  virtual bool on_state_change(char /*new_state*/, char /*old_state*/) {
    return true;
  };

  virtual void in_state(char state) {
    switch (state) {
    case BTN_WAIT_DOWN:
      if (digitalRead(BTN_PIN) == LOW) {
        ledTask.gotoStateForce(ledTask.isOn() ? LED_OFF : LED_ON);
        this->delay(50, BTN_WAIT_UP);
      }
      break;

    case BTN_WAIT_UP:
      if (digitalRead(BTN_PIN) == HIGH) {
        this->delay(50, BTN_WAIT_DOWN);
      }
      break;

    default:
      break;
    };
  };
};

#endif // __BUTTON_H__
