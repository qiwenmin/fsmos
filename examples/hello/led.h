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

#ifndef __LED_H__
#define __LED_H__

#include <fsmos.h>

#define LED_PIN (1)

#define LED_ON (FSM_STATE_USERDEF + 1)
#define LED_OFF (FSM_STATE_USERDEF + 2)

class LedTask : public FsmTask {
public:
  virtual void init() {
    pinMode(LED_PIN, OUTPUT);
    this->_is_on = false;
    this->gotoState(LED_ON);
  };

  virtual bool on_state_change(int8_t new_state, int8_t /*old_state*/) {
    switch(new_state) {
    case LED_ON:
      digitalWrite(LED_PIN, HIGH);
      this->_is_on = true;
      this->delay(2000, LED_OFF);
      break;
    case LED_OFF:
      digitalWrite(LED_PIN, LOW);
      this->_is_on = false;
      this->delay(2000, LED_ON);
      break;
    default:
      break;
    }

    return true;
  };

  virtual void in_state(int8_t) {};

  bool isOn() { return this->_is_on; };
private:
  bool _is_on;
};

#endif // __LED_H__
