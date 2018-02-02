#include <fsmos.h>
#include "pitches.h"

#define TONE_PIN (2)

// notes in the melody:
const int melody[] = {
  NOTE_F5, NOTE_C5, NOTE_C5, NOTE_D5, NOTE_C5, 0, NOTE_E5, NOTE_F5, 0, 0
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
const int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4, 4, 2
};

const int noteCount = sizeof(melody) / sizeof(int);

#define PLAY_NOTE (FSM_STATE_USERDEF + 1)

class MelodyTask: public FsmTask {
public:
  virtual void init() {
    _idx = 0;
    gotoState(PLAY_NOTE);
  };

  virtual bool on_state_change(int8_t new_state, int8_t) {
    if (new_state == PLAY_NOTE) {
      // stop the tone playing:
      noTone(TONE_PIN);

      // to calculate the note duration, take one second divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / noteDurations[_idx];
      tone(TONE_PIN, melody[_idx], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes, PLAY_NOTE);

      _idx ++;
      if (_idx == noteCount) _idx = 0;
    }

    return true;
  };

  virtual void in_state(int8_t) {};
private:
  int _idx;
};

FsmOs fsmOs(1);
MelodyTask melodyTask;

void setup() {
  // put your setup code here, to run once:
  fsmOs.addTask(&melodyTask);

  fsmOs.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  fsmOs.loop();
}
