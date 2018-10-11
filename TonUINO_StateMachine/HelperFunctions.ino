#include <JC_Button.h>

Button pauseButton(buttonPause);
Button upButton(buttonUp);
Button downButton(buttonDown);


// Funktion zum einlesen der Button Zustände
// Hier wird nur das drücken der Buttons ausgelesen.
void SetButtonStates() {
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

void initSerial() {
  Serial.begin(115200); // Es gibt ein paar Debug Ausgaben über die serielle Schnittstelle
  Serial.println(F("TonUINO StateMachine Version 1.0"));
  Serial.println(F("(c) Bastian Hepp & Florian Trück"));
  Serial.println(F("based on TonUINO by Thorsten Voß"));
}

void initButtons() {

  // Knöpfe mit PullUp
  pinMode(buttonPause, INPUT_PULLUP);
  pinMode(buttonUp, INPUT_PULLUP);
  pinMode(buttonDown, INPUT_PULLUP);

  // Busy Pin
  pinMode(busyPin, INPUT);
}
