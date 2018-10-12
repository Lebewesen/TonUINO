
void StateMachinePlayer() {

  switch (playerState) {
    case PAUSE:
      if (pauseButtonState == PRESSED) {
        mp3.start();
      } else if (pauseButtonState == LONGPRESSED) {
        // neue RFID Karte
        globalState = RFIDCARD;
      }
      break;

    case PLAYING:
      //mp3.loop();

      if (pauseButtonState == PRESSED)
        mp3.pause();
      else if (pauseButtonState == LONGPRESSED)
        mp3.playAdvertisement(currentTrack);

      if (upButtonState == PRESSED)
        playerState = NEXT_TRACK;
      else if (upButtonState == LONGPRESSED) {
        Serial.println(F("Volume Up"));
        mp3.increaseVolume();
      }

      if (downButtonState == PRESSED)
        playerState = PREVIOUS_TRACK;
      else if (downButtonState == LONGPRESSED) {
        Serial.println(F("Volume Down"));
        mp3.decreaseVolume();
      }
      break;

    case NEXT_TRACK:
      nextTrack(random(65536)); //TODO: wieso Random?
      break;

    case PREVIOUS_TRACK:
      previousTrack();
      break;
  }
}
