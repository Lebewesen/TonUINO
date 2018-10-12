#include <JC_Button.h>

Button pauseButton(buttonPause);
Button upButton(buttonUp);
Button downButton(buttonDown);

void setButtonState() {
  pauseButton.read();
  upButton.read();
  downButton.read();

  // PAUSE BUTTON
  if (pauseButton.wasReleased())
    pauseButtonState = PRESSED;
  else if (pauseButton.pressedFor(LONG_PRESS))
    pauseButtonState = LONGPRESSED;
  else
    pauseButtonState = RELEASED;

  // NEXT BUTTON
  if (upButton.wasReleased())
    upButtonState = PRESSED;
  else if (upButton.pressedFor(LONG_PRESS))
    upButtonState = LONGPRESSED;
  else
    upButtonState = RELEASED;

  // PREVIOUS BUTTON
  if (downButton.wasReleased())
    downButtonState = PRESSED;
  else if (downButton.pressedFor(LONG_PRESS))
    downButtonState = LONGPRESSED;
  else
    downButtonState = RELEASED;
}

// Funktion zum einlesen der Button Zustände
// Hier wird nur das drücken der Buttons ausgelesen.
void SetStates() {
  playerState = isPlaying() ? PLAYING : PAUSE; // Read player state
  setButtonState();
  // Global State
  if (mfrc522.PICC_IsNewCardPresent())
    globalState = RFIDCARD;
  else
    globalState = NORMAL;
}

void initSerial() {
  Serial.begin(SERIAL_BAUD); // Es gibt ein paar Debug Ausgaben über die serielle Schnittstelle
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

bool isPlaying() {
  return !digitalRead(busyPin);
}

void waitForPlayFinish() {
  do {
    delay(10);
  } while (isPlaying());
}

void menue(int playTrack, bool preview, int previewFromFolder, int returnValue) {
  mp3.playMp3FolderTrack(playTrack);
  delay(1000);
  if (preview) {
    waitForPlayFinish();
    if (previewFromFolder == 0)
      mp3.playFolderTrack(returnValue, 1);
    else
      mp3.playFolderTrack(previewFromFolder, returnValue);
  }
}

int voiceMenu(int numberOfOptions, int startMessage, int messageOffset,
              bool preview = false, int previewFromFolder = 0) {
  int returnValue = 0;
  if (startMessage != 0)
    mp3.playMp3FolderTrack(startMessage);

  do {
    setButtonState();
    mp3.loop();

    if (pauseButtonState == PRESSED) {
      if (returnValue != 0)
        return returnValue;
      delay(1000);
    }

    switch (upButtonState) {
      case LONGPRESSED:
        returnValue = min(returnValue + 10, numberOfOptions);
        menue(messageOffset + returnValue, preview, previewFromFolder, returnValue);
        break;
      case RELEASED:
        returnValue = min(returnValue + 1, numberOfOptions);
        menue(messageOffset + returnValue, preview, previewFromFolder, returnValue);
        break;
    }

    switch (downButtonState) {
      case LONGPRESSED:
        returnValue = max(returnValue - 10, 1);
        menue(messageOffset + returnValue, preview, previewFromFolder, returnValue);
        break;
      case RELEASED:
        returnValue = max(returnValue - 1, 1);
        menue((messageOffset + returnValue), preview, previewFromFolder, returnValue);
        break;
    }

  } while (true);
}
