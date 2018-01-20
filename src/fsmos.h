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

#include <stdlib.h>

#ifndef ARDUINO
unsigned long millis();
#else // ARDUINO
#include <arduino.h>
#endif // ARDUINO


#define FSM_STATE_INVALID (-1)
#define FSM_STATE_INITIALIZED (0)
#define FSM_STATE_USERDEF (10)

class FsmTask {
    friend class FsmOs;
public:
    FsmTask() :
        _state(FSM_STATE_INVALID),
        _new_state(FSM_STATE_INVALID),
        _is_waiting(false),
        _waiting_start_at(0),
        _waiting_duration(0),
        _waiting_finish_state(FSM_STATE_INVALID) {

    };

    virtual void init() = 0;

    virtual bool on_state_change(int8_t new_state, int8_t old_state) = 0;
    virtual void in_state(int8_t state) = 0;

    void gotoState(int8_t new_state) {
        this->_new_state = new_state;
    };

    void gotoStateForce(int8_t new_state) {
        if (this->_is_waiting) {
            this->_is_waiting = false;
            this->_waiting_start_at = 0;
            this->_waiting_duration = 0;
            this->_waiting_finish_state = FSM_STATE_INVALID;
        }

        this->_new_state = new_state;
    };
protected:
    void delay(unsigned long timeout, int8_t newState) {
        this->_is_waiting = true;

        this->_waiting_start_at = millis();
        this->_waiting_duration = timeout;
        this->_waiting_finish_state = newState;
    };
private:
    int8_t _state;
    int8_t _new_state;

    bool _is_waiting;
    unsigned long _waiting_start_at;
    unsigned long _waiting_duration;
    int8_t _waiting_finish_state;

    // return true if waiting for timeout
    bool processWaiting() {
        if (!this->_is_waiting) {
            return false;
        }

        if (millis() - this->_waiting_start_at < this->_waiting_duration) {
            return true;
        }

        this->_new_state = this->_waiting_finish_state;

        this->_is_waiting = false;
        this->_waiting_start_at = 0;
        this->_waiting_duration = 0;
        this->_waiting_finish_state = FSM_STATE_INVALID;

        return false;
    };
};

class FsmOs {
public:
    FsmOs(int max_task_count) {
        _tasks = (FsmTask **)malloc(sizeof(FsmTask *) * max_task_count);
        _task_count = 0;
        _max_task_count = max_task_count;

        for (int i = 0; i < _max_task_count; i ++) {
            _tasks[i] = 0;
        }
    };

    virtual ~FsmOs() {
        free((void *)_tasks);
    };

    int addTask(FsmTask *task) {
        if (_max_task_count == _task_count) {
            return -1;
        }

        int freeIdx = findTaskByPtr(0);

        if (freeIdx == -1) {
            return -1;
        }

        _tasks[freeIdx] = task;
        _task_count ++;

        return freeIdx;
    };

    int deleteTask(FsmTask *task) {
        int result = findTaskByPtr(task);

        if (result != -1) {
            _tasks[result] = 0;
            _task_count --;
        }

        return result;
    };

    void init() {
        for (int i = 0; i < _max_task_count; i ++) {
            FsmTask *task = _tasks[i];

            if (task && task->_state == FSM_STATE_INVALID) {
                task->init();
                task->_state = FSM_STATE_INITIALIZED;
            }
        }
    };

    void loop() {
        for (int i = 0; i < _max_task_count; i ++) {
            FsmTask *task = _tasks[i];

            if (task) {
                if (!task->processWaiting()) {
                    if (task->_new_state != FSM_STATE_INVALID) {
                        if (task->on_state_change(task->_new_state, task->_state)) {
                            task->_state = task->_new_state;
                            task->_new_state = FSM_STATE_INVALID;
                        }
                    }

                    task->in_state(task->_state);
                }
            }
        }
    };
private:
    FsmTask **_tasks;
    int _task_count;
    int _max_task_count;

    int findTaskByPtr(FsmTask *task) {
        int result = -1;

        for (int i = 0; i < _max_task_count; i ++) {
            if (_tasks[i] == task) {
                result = i;
                break;
            }
        }

        return result;
    };
};

#endif // __FSMOS_H__
