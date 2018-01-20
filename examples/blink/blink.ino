#include <fsmos.h>
#include "led_def.h"

#define LED_OFF (FSM_STATE_USERDEF + 1)
#define LED_ON (FSM_STATE_USERDEF + 2)

class Blinker : public FsmTask {
public:
  virtual void init() {
    pinMode(LED_PIN, OUTPUT);
    this->gotoState(LED_ON);
  };

  virtual bool on_state_change(char new_state, char) {
    if (new_state == LED_OFF) {
      digitalWrite(LED_PIN, LOW);
      this->delay(500, LED_ON);
    } else if (new_state == LED_ON) {
      digitalWrite(LED_PIN, HIGH);
      this->delay(500, LED_OFF);
    }

    return true;
  };

  virtual void in_state(char) {};
};

Blinker blinker;
FsmOs fsmOs(1);

void setup() {
  fsmOs.addTask(&blinker);
  fsmOs.init();
}

void loop() {
  fsmOs.loop();
}

