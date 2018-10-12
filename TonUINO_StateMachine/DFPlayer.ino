
void initDFPlayer( byte startingVolume) {
  // DFPlayer Mini initialisieren
  mp3.begin();
  mp3.setVolume(startingVolume);
}

static void nextTrack(uint16_t track) {
  if (track == _lastTrackFinished || globalState == RFIDCARD)
    return;

  _lastTrackFinished = track;
  switch (playmode) {
    case  AUDIODRAMA:
    case  SINGLE:
      Serial.println(F("Einzel-/Hörspielmodus ist aktiv -> keinen neuen Track spielen"));
      if (SLEEP_ENABLED)
        mp3.sleep();
      break;
    case  PARTY:
      currentTrack = random(1, numTracksInFolder + 1);
      Serial.print(F("Party Modus ist aktiv -> zufälligen Track spielen: "));
      Serial.println(currentTrack);
      mp3.playFolderTrack(myCard.folder, currentTrack);
      break;
    case  AUDIOBOOK:
    case  FOLDER:
      if (currentTrack != numTracksInFolder) {
        currentTrack = currentTrack + 1;
        mp3.playFolderTrack(myCard.folder, currentTrack);
        Serial.print(F("Hörbuch-/Albummodus ist aktiv -> nächster Track: "));
        Serial.print(currentTrack);
        if (playmode == AUDIOBOOK)
          EEPROM.write(myCard.folder, currentTrack);
      } else {
        if (playmode == AUDIOBOOK)
          EEPROM.write(myCard.folder, 1);
        if (SLEEP_ENABLED)
          mp3.sleep();
      }
      break;
  }
}

static void previousTrack() {
  switch (playmode) {
    case  AUDIODRAMA:
    case  SINGLE:
    case  PARTY:
      Serial.println(F("Party/Einzel/Hörspielmodus Modus ist aktiv -> Track von vorne spielen"));
      break;
    case  AUDIOBOOK:
    case  FOLDER:
      Serial.println(F("Albummodus/Hörbuch ist aktiv -> vorheriger Track"));
      if (currentTrack != 1) {
        currentTrack = currentTrack - 1;
        if (playmode == AUDIOBOOK)
          EEPROM.write(myCard.folder, currentTrack); // Fortschritt im EEPROM abspeichern
      }
      break;
  }
  mp3.playFolderTrack(myCard.folder, currentTrack);
}
