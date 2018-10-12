MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;
bool successRead;
byte sector = 1;
byte blockAddr = 4;
byte trailerBlock = 7;

void initMFR() { // MFR  initialisieren
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}

void CardHandling() {
  globalState = NORMAL;
  if (!mfrc522.PICC_ReadCardSerial())
    return;
    
  if (readCard(&myCard) == true) {
    if (myCard.cookie == 322417479 && myCard.folder != 0 && myCard.mode != 0) {
      _lastTrackFinished = 0;
      numTracksInFolder = mp3.getFolderTrackCount(myCard.folder);
      Serial.print(numTracksInFolder);
      Serial.print(F(" Dateien in Ordner "));
      Serial.println(myCard.folder);

      switch (myCard.mode) {
        case 1: // Hörspielmodus: eine zufällige Datei aus dem Ordner
          Serial.println(F("Hörspielmodus -> zufälligen Track wiedergeben "));
          Serial.println(currentTrack);
          playmode = AUDIODRAMA;
          currentTrack = random(1, numTracksInFolder + 1);
          break;
        case 2: // Album Modus: kompletten Ordner spielen
          Serial.println(F("Album Modus -> kompletten Ordner wiedergeben"));
          playmode = FOLDER;
          currentTrack = 1;
          break;
        case 3: // Party Modus: Ordner in zufälliger Reihenfolge
          Serial.println(F("Party Modus -> Ordner in zufälliger Reihenfolge wiedergeben"));
          playmode = PARTY;
          currentTrack = random(1, numTracksInFolder + 1);
          break;
        case 4: // Einzel Modus: eine Datei aus dem Ordner abspielen
          Serial.println(F("Einzel Modus -> eine Datei aus dem Odrdner abspielen"));
          playmode = SINGLE;
          currentTrack = myCard.special;
          break;
        case 5: // Hörbuch Modus: kompletten Ordner spielen und Fortschritt merken
          Serial.println(F("Hörbuch Modus -> kompletten Ordner spielen und Fortschritt merken"));
          playmode = AUDIOBOOK;
          currentTrack = EEPROM.read(myCard.folder);
          break;
      }
      mp3.playFolderTrack(myCard.folder, currentTrack);
    } else
      setupCard();// Neue Karte konfigurieren
  }
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

}

void resetCard() {
  do {
    pauseButton.read();
    upButton.read();
    downButton.read();
    if (upButton.wasReleased() || downButton.wasReleased()) {
      Serial.print(F("Abgebrochen!"));
      mp3.playMp3FolderTrack(802);
      return;
    }
  } while (!mfrc522.PICC_IsNewCardPresent()); //Solange keine Karte aufgelegt wurde kann der Vorgang abgebrochen werden
  if (!mfrc522.PICC_ReadCardSerial())
    return;
  Serial.print(F("Karte wird neu Konfiguriert!"));
  setupCard();
}

void setupCard() {
  mp3.pause();
  Serial.print(F("Neue Karte konfigurieren"));
  myCard.folder = voiceMenu(99, 300, 0, true); // Ordner abfragen
  myCard.mode = voiceMenu(6, 310, 310);  // Wiedergabemodus abfragen

  switch (myCard.mode) {
    case 5: // Hörbuchmodus -> Fortschritt im EEPROM auf 1 setzen
      EEPROM.write(myCard.folder, 1);
      break;
    case 4: // Einzelmodus -> Datei abfragen
      myCard.special = voiceMenu(mp3.getFolderTrackCount(myCard.folder), 320, 0, true, myCard.folder);
      break;
    case 6: // Admin Funktionen
      myCard.special = voiceMenu(3, 320, 320);
  }
  writeCard(myCard); // Karte ist konfiguriert -> speichern
}

bool readCard(nfcTagObject * nfcTag) {
  bool returnValue = true;
  // Show some details of the PICC (that is: the tag/card)
  Serial.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();
  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));

  byte buffer[18];
  byte size = sizeof(buffer);

  // Authenticate using key A
  Serial.println(F("Authenticating using key A..."));
  status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(
             MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    returnValue = false;
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Show the whole sector as it currently is
  Serial.println(F("Current data in sector:"));
  mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
  Serial.println();

  // Read data from the block
  Serial.print(F("Reading data from block "));
  Serial.print(blockAddr);
  Serial.println(F(" ..."));
  status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(blockAddr, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    returnValue = false;
    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  }
  Serial.print(F("Data in block "));
  Serial.print(blockAddr);
  Serial.println(F(":"));
  dump_byte_array(buffer, 16);
  Serial.println();
  Serial.println();

  uint32_t tempCookie;
  tempCookie = (uint32_t)buffer[0] << 24;
  tempCookie += (uint32_t)buffer[1] << 16;
  tempCookie += (uint32_t)buffer[2] << 8;
  tempCookie += (uint32_t)buffer[3];

  nfcTag->cookie = tempCookie;
  nfcTag->version = buffer[4];
  nfcTag->folder = buffer[5];
  nfcTag->mode = buffer[6];
  nfcTag->special = buffer[7];

  return returnValue;
}

void writeCard(nfcTagObject nfcTag) {
  MFRC522::PICC_Type mifareType;
  byte buffer[16] = {0x13, 0x37, 0xb3, 0x47, // 0x1337 0xb347 magic cookie to
                     // identify our nfc tags
                     0x01,                   // version 1
                     nfcTag.folder,          // the folder picked by the user
                     nfcTag.mode,    // the playback mode picked by the user
                     nfcTag.special, // track or function for admin cards
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };
  byte size = sizeof(buffer);
  mifareType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  // Authenticate using key B
  Serial.println(F("Authenticating again using key B..."));
  status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(
             MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    mp3.playMp3FolderTrack(401);
    return;
  }
  // Write data to the block
  Serial.print(F("Writing data into block "));
  Serial.print(blockAddr);
  Serial.println(F(" ..."));
  dump_byte_array(buffer, 16);
  Serial.println();
  status = (MFRC522::StatusCode)mfrc522.MIFARE_Write(blockAddr, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    mp3.playMp3FolderTrack(401);
  }
  else
    mp3.playMp3FolderTrack(400);
  Serial.println();
  delay(100);
}

// Helper routine to dump a byte array as hex values to Serial.
void dump_byte_array(byte * buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
