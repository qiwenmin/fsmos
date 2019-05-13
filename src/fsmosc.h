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

#ifndef __FSMOS_H__
#define __FSMOS_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef ARDUINO
uint32_t millis();
#endif // ARDUINO

#define FO_STATE_INVALID (-1)
#define FO_STATE_INIT (0)
#define FO_STATE_TASK_DEF_BEGIN (1)

typedef int8_t FO_STATE;

struct fo_task;
typedef struct fo_task *fo_task_ptr;

typedef void (*fo_task_func)(fo_task_ptr t, FO_STATE state, FO_STATE new_state);

struct fo_task {
  fo_task_func _t_func;
  FO_STATE _state, _new_state;
  unsigned long _delay_at;
  unsigned long _delay;
  FO_STATE _delay_state;
  void *data;
};

typedef uint8_t FO_TASK_SIZE_TYPE;

#define FO_INIT(max_tasks) \
    struct fo_task fo_tasks[max_tasks];\
    FO_TASK_SIZE_TYPE fo_tasks_capacity = max_tasks;

fo_task_ptr fo_create_task(fo_task_func t_func);
void fo_loop();
void fo_goto_state(fo_task_ptr t, FO_STATE state);
void fo_goto_state_force(fo_task_ptr t, FO_STATE state);
void fo_delay(fo_task_ptr t, unsigned long delay, FO_STATE state);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __FSMOS_H__

