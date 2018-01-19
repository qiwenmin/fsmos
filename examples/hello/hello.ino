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

#include <fsmos.h>
#include "led.h"
#include "button.h"

FsmOs fsmos(2);

LedTask ledTask;
ButtonTask btnTask;

void setup() {
  // put your setup code here, to run once:
  fsmos.addTask(&ledTask);
  fsmos.addTask(&btnTask);

  fsmos.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  fsmos.loop();
}
