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

#include "fsmosc.h"

// SDCC STM8
#ifdef __SDCC_stm8

#include "stm8s.h"

#define set_bit(register_8, bit) (register_8 |= (1 << bit))
#define clear_bit(register_8, bit) (register_8 &= ~(1 << bit))
#define toggle_bit(register_8, bit) (register_8 ^= (1 << bit))

static volatile uint32_t __global_millis = 0;

static void timer4_millis_isr() __interrupt(TIM4_ISR) {
    __global_millis ++;
    clear_bit(TIM4_SR, TIM4_SR_UIF);
}

static void init_timer() {
    enable_interrupts();
    TIM4_PSCR = 3;
    TIM4_ARR = 249;
    set_bit(TIM4_IER, TIM4_IER_UIE);
    set_bit(TIM4_CR1, TIM4_CR1_CEN);
}

uint32_t millis() {
    return __global_millis;
}

#endif // __SDCC_stm8

// FSMOSC
extern struct fo_task fo_tasks[];
extern FO_TASK_SIZE_TYPE fo_tasks_capacity;
static FO_TASK_SIZE_TYPE fo_tasks_count = 0;

fo_task_ptr fo_create_task(fo_task_func t_func) {
  fo_task_ptr t = 0;

  if (fo_tasks_count < fo_tasks_capacity) {
    t = &fo_tasks[fo_tasks_count ++];

    t->_t_func = t_func;
    t->_state = FO_STATE_INVALID;
    t->_new_state = FO_STATE_INIT;
    t->_delay_at = 0;
    t->_delay = 0;
    t->_delay_state = FO_STATE_INVALID;
    t->data = 0;
  }

  return t;
}

static inline void disable_delay(fo_task_ptr t) {
  t->_delay_at = 0;
  t->_delay = 0;
  t->_delay_state = FO_STATE_INVALID;
}

void fo_loop() {
  FO_TASK_SIZE_TYPE i;
  for (i = 0; i < fo_tasks_count; i ++) {
    fo_task_ptr t = &fo_tasks[i];

    if (t->_t_func) {
      if (t->_delay_state == FO_STATE_INVALID) {
        // Not in delay
        FO_STATE new_state = t->_new_state;
        t->_new_state = FO_STATE_INVALID;

        t->_t_func(t, t->_state, new_state);
        if (new_state != FO_STATE_INVALID) {
          t->_state = new_state;
        }
      } else {
        // In delay
        if (millis() - t->_delay_at >= t->_delay) {
          t->_new_state = t->_delay_state;
          disable_delay(t);
        }
      }
    }
  }
}

void fo_goto_state(fo_task_ptr t, FO_STATE state) {
  t->_new_state = state;
}

void fo_goto_state_force(fo_task_ptr t, FO_STATE state) {
  disable_delay(t);

  t->_new_state = state;
}

void fo_delay(fo_task_ptr t, unsigned long delay, FO_STATE state) {
  t->_delay_at = millis();
  t->_delay = delay;
  t->_delay_state = state;
}

extern void setup();
extern void loop();

#ifndef ARDUINO
int main() {
  init_timer();

  setup();

  for (;;) loop();
}
#endif // ARDUINO
