#include <fsmos.h>
#include "led_def.h"

#define LED_ON (FSM_STATE_USERDEF + 1)
#define LED_OFF (FSM_STATE_USERDEF + 2)

class FadeTask : public FsmTask {
public:
  virtual void init() {
    _onOffTimeout = 25;
    _step = 1;
    _onTimeout = 0;

    pinMode(LED_PIN, OUTPUT);
    this->gotoState(LED_ON);
  };

  virtual bool on_state_change(int8_t new_state, int8_t) {
    if (new_state == LED_ON) {
      digitalWrite(LED_PIN, HIGH);
      this->delay(this->_onTimeout, LED_OFF);
    } else if (new_state == LED_OFF) {
      digitalWrite(LED_PIN, LOW);
      this->delay(this->_onOffTimeout - this->_onTimeout, LED_ON);      
    }

    this->_onTimeout += this->_step;
    if ((this->_onTimeout == 0) || (this->_onTimeout == this->_onOffTimeout)) {
      this->_step = - this->_step;
    }
    return true;
  };

  virtual void in_state(int8_t) {};
private:
  uint8_t _step;
  uint8_t _onOffTimeout;
  uint8_t _onTimeout;
};

FsmOs fsmOs(1);
FadeTask fadeTask;

void setup() {
  // put your setup code here, to run once:
  fsmOs.addTask(&fadeTask);

  fsmOs.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  fsmOs.loop();
}
