#include <DFMiniMp3.h>
#include <EEPROM.h>
#include <MFRC522.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include "enums.h"
#include "mp3Notify.h"

//------------------------------------
// Pin Definitions
// DFPlayer Mini
#define DFPlayer_StartingVolume 15
#define DFPlayer_RX 2
#define DFPlayer_TX 3
#define busyPin 4

// MFRC522
#define RST_PIN 9                 // Configurable, see typical pin layout above
#define SS_PIN 10                 // Configurable, see typical pin layout above

// Buttons
#define buttonPause A0
#define buttonUp A1
#define buttonDown A2
#define LONG_PRESS 1000

// Max Volume
byte maxVolume = 30;

//------------------------------------
// StateMachine conditions
state_pg globalState = NORMAL;
state_p playerState = PAUSE;

// Button conditions
state_b b1State = RELEASED;
state_b b2State = RELEASED;
state_b b3State = RELEASED;

// MP3 Modus
playback_mode playmode = SINGLE;

//------------------------------------
// Init DFPlayer and NFC Object
SoftwareSerial mySoftwareSerial(DFPlayer_RX, DFPlayer_TX); // RX, TX
static DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(mySoftwareSerial);
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522

void setup() {
  randomSeed(analogRead(A0)); // random seed initialize
  initSerial();
  initButtons();
  initDFPlayer(DFPlayer_StartingVolume);
  initMFR();
}

void loop() {
  SetButtonStates();
  StateMachineGlobal();
}
