#include "enums.h"

// StateMachine Zustände
state_pg globalState = NORMAL;
state_p playerState = PAUSE;

// Button Zustände
state_b b1State = RELEASED;
state_b b2State = RELEASED;
state_b b3State = RELEASED;

// MP3 Modus
mode_mp3 playmode = SINGLE;


void setup() {
  // Knöpfe mit PullUp
  pinMode(buttonPause, INPUT_PULLUP);
  pinMode(buttonUp, INPUT_PULLUP);
  pinMode(buttonDown, INPUT_PULLUP);

  // Busy Pin
  pinMode(busyPin, INPUT);

}

void loop() {
  SetButtonStates();  
  StateMachineGlobal();
}
