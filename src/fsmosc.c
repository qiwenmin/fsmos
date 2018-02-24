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

#include <stdlib.h>
#include <stdio.h>
#include <Arduino.h>

#include "fsmosc.h"

static fo_task_ptr *fo_tasks = 0;
static FO_TASK_SIZE_TYPE fo_tasks_capacity = 0;
static FO_TASK_SIZE_TYPE fo_tasks_count = 0;

void fo_init(FO_TASK_SIZE_TYPE max_tasks) {
  fo_tasks = malloc(sizeof(fo_task_ptr) * max_tasks);
  if (fo_tasks) {
    fo_tasks_capacity = max_tasks;
  }
}


#define FO_DEFAULT_TASKS_CAPACITY (16)

static FO_TASK_SIZE_TYPE make_enought_tasks_space() {
#if defined(TASK_TABLE_EXPENDABLE)
  fo_task_ptr *new_fo_tasks;
  if (fo_tasks_capacity == fo_tasks_count) {
    FO_TASK_SIZE_TYPE new_capacity = fo_tasks_capacity << 1;
  
    if (fo_tasks_capacity == 0) {
      new_capacity = FO_DEFAULT_TASKS_CAPACITY;
    }

    new_fo_tasks = realloc(fo_tasks, new_capacity);
    if (new_fo_tasks) {
      fo_tasks = new_fo_tasks;
      fo_tasks_capacity = new_capacity;
    }
  }
#endif // TASK_TABLE_EXPENDABLE

  return fo_tasks_capacity - fo_tasks_count;
}

static void fo_add_task(fo_task_ptr t) {
  if (fo_tasks_count < fo_tasks_capacity) {
    fo_tasks[fo_tasks_count ++] = t;
  }
}

fo_task_ptr fo_create_task(fo_task_func t_func) {
  fo_task_ptr t = 0;

  if (make_enought_tasks_space()) {
    t = malloc(sizeof(struct fo_task));

    if (t) {
      t->_t_func = t_func;
      t->_state = FO_STATE_INVALID;
      t->_new_state = FO_STATE_INIT;
      t->_delay_at = 0;
      t->_delay = 0;
      t->_delay_state = FO_STATE_INVALID;
      t->data = 0;

      fo_add_task(t);
    }
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
    fo_task_ptr t = fo_tasks[i];

    if (t) {
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

