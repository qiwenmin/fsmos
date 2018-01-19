// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this program.  If not, see
// <http://www.gnu.org/licenses/>.

// This demo runs on linux/macOS, is not for Arduino
#ifndef ARDUINO

#include <stdio.h>
#include "fsmos.h"

#define DEBUG(msg) fprintf(stderr, "%s", msg)


static FsmOs fmsos(3);

#define LED_OFF (FSM_STATE_USERDEF + 1)
#define LED_ON (FSM_STATE_USERDEF + 2)

class LedTask : public FsmTask {
public:
    virtual void init() {
        this->_is_on = false;

        this->gotoState(LED_ON);
    };

    virtual bool on_state_change(char new_state, char old_state) {
        switch (new_state) {
        case LED_ON:
            DEBUG("LED ON.\n");
            this->delay(500, LED_OFF);
            this->_is_on = true;
            break;
        case LED_OFF:
            DEBUG("LED OFF.\n");
            this->delay(500, LED_ON);
            this->_is_on = false;
            break;
        default:
            break;
        }

        return true;
    };

    virtual void in_state(char) {};

    bool isOn() { return this->_is_on; };
private:
    bool _is_on;
};

static LedTask ledTask;

#define BTN_UP (FSM_STATE_USERDEF + 1)
#define BTN_DOWN (FSM_STATE_USERDEF + 2)

class ButtonTask : public FsmTask {
public:
    virtual void init() {
        this->gotoState(BTN_UP);
    };

    virtual bool on_state_change(char new_state, char old_state) {
        switch (new_state) {
        case BTN_UP:
            DEBUG("BTN UP.\n");
            this->delay(1100, BTN_DOWN);
            break;
        case BTN_DOWN:
            DEBUG("BTN DOWN.\n");
            ledTask.gotoStateForce(ledTask.isOn() ? LED_OFF : LED_ON);
            this->delay(150, BTN_UP);
            break;
        default:
            break;
        }
        return true;
    };

    virtual void in_state(char) {};
};

static ButtonTask buttonTask;

void setup()
{
    fmsos.addTask(&ledTask);
    fmsos.addTask(&buttonTask);

    fmsos.init();
}

void loop()
{
    fmsos.loop();
}

#endif // ARDUINO
