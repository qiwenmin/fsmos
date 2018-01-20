# FSMOS

FSMOS is a tiny and simple finite-state machine based embedded operating
system. Currently, it supports Arduino and Linux/macOS platforms.

The codes under `examples` folder demonstrate how to use FSMOS.

# About Finite-State Machine

* https://en.wikipedia.org/wiki/Finite-state_machine

# Using FSMOS

Before using FSMOS in Arduino IDE, it need be installed as an Arduino library:

* Clone this project under the Arduino libraries folder (such as
  `~/Documents/Arduino/libraries` on macOS). Or
* Download this project as a ZIP package and usage the menu `Sketch/Include
  Library/Add .ZIP Library...` to install it.

Once FSMOS installed, you can use it in your code (by `#include <fsmos.h>`).

* Declare an instance of FsmOs. This instance manages and schedules all the
  tasks. The tasks are the subclasses of FsmTask and are based on the
  finite-state machine model.
* Define and declare the tasks. Add them to the instance of FsmOs declared
  previously.
* Call the `init()` method of the FsmOs instance in `setup` function to
  initialize the tasks.
* Call the `loop()` method of the FsmOs instance in `loop` function to schedule
  the tasks.

The sample codes show as follows:

```c++
#include <fsmos.h>

class Task1 : public FsmTask {
    // ...
};

class Task2 : public FsmTask {
    // ...
};

// ...

class TaskN : public FsmTask {
    // ...
};

FsmOs fsmOs(N); // initialize FsmOs for N tasks

Task1 task1;
Task2 task2;
// ...
TaskN taskN;

void setup() {
    // Add tasks into fsmOs
    fsmOs.addTask(task1);
    fsmOs.addTask(task2);
    // ...
    fsmOs.addTask(taskN);

    // Initialize all tasks in fsmOs
    fsmOs.init();
}

void loop() {
    // let fsmOs schedules the tasks repeatedly
    fsmOs.loop();
}
```

There are some virtual methods should be implemented when defining the subclass
of FsmTask:

* `virtual void init()` - when executing `init()` method of the FsmOs instance,
  this method is called. The initialization codes of the task should be in this
  method.
* `virtual bool on_state_change(int8_t new_state, int8_t old_state)` - while task
  scheduling, if the task should change from one state to another, this method
  is called.
  * `new_state` - the state will be changed to
  * `old_state` - the state will be changed from
  * returns - if the task accepts the change, `true` should be returned,
    otherwise `false` should be returned. Under normal circumstances, `true`
    should be returned.
* `virtual void in_state(int8_t state)` - while scheduling, this method is called
  repeatedly with the current state as the argument.

In addition, FsmTask also provides some methods for changing state:

* `void gotoState(int8_t state)` - change to the new state. If the task is
  waiting for a delay timeout, this method does nothing.
* `void gotoStateForce(int8_t state)` - change to the new state forcibly. If the
  task is waiting for a delay timeout, the delay is canceled and the new
  state is set.
* `void delay(unsigned long timeout, int8_t new_state)` - change to the new state
  after the delay of `timeout` milliseconds.

[hello project](https://github.com/qiwenmin/fsmos/tree/master/examples/hello)
shows how to use FsmTask and FsmOs：

* [LedTask](https://github.com/qiwenmin/fsmos/blob/master/examples/hello/led.h)
  demonstrates how to use `init`, `on_state_change`, `gotoState` and `delay`.
* [ButtonTask](https://github.com/qiwenmin/fsmos/blob/master/examples/hello/button.h)
  demonstrates how to use `init`, `in_state`, `gotoState`, `gotoStateForce` and
  `delay`.
* [hello.ino](https://github.com/qiwenmin/fsmos/blob/master/examples/hello/hello.ino)
  demonstrates how to add, initialize and schedule the tasks.