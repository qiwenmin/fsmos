# FSMOS

FSMOS是一个基于有限状态机（FMS）的嵌入式操作系统，简单小巧。目前支持Arduino和Linux/macOS平台。

`examples`目录中的代码演示了如何使用FSMOS。

## 关于有限状态机

* https://zh.wikipedia.org/wiki/%E6%9C%89%E9%99%90%E7%8A%B6%E6%80%81%E6%9C%BA
* https://en.wikipedia.org/wiki/Finite-state_machine

## 使用FSMOS

在Arduino上使用FSMOS，需要将这个项目安装为Arduino的库。可以任选下面的一种方法安装：

* 将本项目clone到Arduino的库目录下。如macOS中的`~/Documents/Arduino/libraries`目录下。
* 下载本项目的zip包，用Arduino IDE菜单中的`Sketch/Include Library/Add .ZIP Library...`安装。

安装后，即可在代码中（通过`#include <fsmos.h>`）使用FSMOS。

* 首先需要一个FsmOs实例。这个实例用来管理和调度所有的任务（FsmTask的子类：依照有限状态机模型来实现）。
* 然后创建任务，并将任务添加到FsmOs实例中。
* 然后调用FsmOs实例的`init()`方法，完成所有任务的初始化。
* 最后在`loop`函数中调用FsmOs实例的`loop()`方法，使其进行任务调度。

大致的代码结构如下所示：

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
    // let fsmOs schedules the tasks repeatly
    fsmOs.loop();
}
```

定义FsmTask的子类时，需要实现下列三个方法：

* `virtual void init()` - 执行FsmOs实例的`init()`方法时被调用。这里适合写任务的初始化代码。
* `virtual bool on_state_change(char new_state, char old_state)` - 在任务调度被调度时，如果需要进行状态迁移，此方法被调用。
  * `new_state` - 要迁移到的新状态
  * `old_state` - 老状态
  * 返回值 - 如果允许FsmOs将任务设置为新状态，返回`true`，否则返回`false`。正常情况下都应该返回`true`。
* `virtual void in_state(char state)` - 任务调度过程中，此方法会被重复调用，参数为任务的当前状态。

此外，任务还提供了一些状态迁移的方法：

* `void gotoState(char state)` - 将任务迁移到新的状态。如果任务当前正在延时等待中，调用此方法无效果。
* `void gotoStateForce(char state)` - 将任务迁移到新的状态。如果任务当前正在延时等待中，则立即取消延时等待，并设置新的状态。
* `void delay(unsigned long timeout, char new_state)` - 延时`timeout`毫秒后，将任务迁移到`new_state`状态。

[hello项目](https://github.com/qiwenmin/fsmos/tree/master/examples/hello)演示了如何使用FsmTask和FsmOs：

* [LedTask](https://github.com/qiwenmin/fsmos/blob/master/examples/hello/led.h)演示了`init`、`on_state_change`、`gotoState`和`delay`的用法。
* [ButtonTask](https://github.com/qiwenmin/fsmos/blob/master/examples/hello/button.h)演示了`init`、`in_state`、`gotoStateForce`和`delay`的用法。
* [主程序](https://github.com/qiwenmin/fsmos/blob/master/examples/hello/hello.ino)演示了添加、初始化和调度任务。
