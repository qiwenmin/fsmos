#include <fsmosc.h>
#include "led_def.h"

enum {
  STATE_LED_ON = FO_STATE_TASK_DEF_BEGIN,
  STATE_LED_OFF
};

void blink_task_f(fo_task_ptr t, FO_STATE, FO_STATE new_state) {
  switch (new_state) {
  case FO_STATE_INIT:
    pinMode(LED_PIN, OUTPUT);
    fo_goto_state(t, STATE_LED_ON);
    break;
  case STATE_LED_ON:
    digitalWrite(LED_PIN, HIGH);
    fo_delay(t, 500, STATE_LED_OFF);
    break;
  case STATE_LED_OFF:
    digitalWrite(LED_PIN, LOW);
    fo_delay(t, 500, STATE_LED_ON);
    break;
  case FO_STATE_INVALID:
    break;
  default:
    break;
  }
}

void setup() {
  fo_init(1);

  fo_create_task(blink_task_f);
}

void loop() {
  fo_loop();
}
