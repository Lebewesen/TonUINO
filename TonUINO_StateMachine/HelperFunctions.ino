#include <JC_Button.h>

#define buttonPause A0
#define buttonUp A1
#define buttonDown A2
#define busyPin 4

#define LONG_PRESS 1000

Button pauseButton(buttonPause);
Button upButton(buttonUp);
Button downButton(buttonDown);


// Funktion zum einlesen der Button Zustände
// Hier wird nur das drücken der Buttons ausgelesen.
void SetButtonStates()
{
  pauseButton.read();
  upButton.read();
  downButton.read();

  // Read player state
  playerState = !digitalRead(busyPin) ? PLAYING : PAUSE;

  // PAUSE BUTTON
  if (pauseButton.wasReleased())
    b1State = PRESSED;
  else if (pauseButton.pressedFor(LONG_PRESS))
    b1State = LONGPRESSED;
  else
    b1State = RELEASED;

  // NEXT BUTTON
  if (upButton.wasReleased())
    b2State = PRESSED;
  else if (upButton.pressedFor(LONG_PRESS))
    b2State = LONGPRESSED;
  else
    b2State = RELEASED;

  // PREVIOUS BUTTON
  if (downButton.wasReleased())
    b3State = PRESSED;
  else if (downButton.pressedFor(LONG_PRESS))
    b3State = LONGPRESSED;
  else
    b3State = RELEASED;
}
