#include <DFMiniMp3.h>
#include <EEPROM.h>
#include <MFRC522.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include "enums.h"
#include "objects.h"


//------------------------------------
// Pin Definitions
// DFPlayer Mini
#define DFPlayer_StartingVolume 15
#define DFPlayer_RX 2
#define DFPlayer_TX 3
#define busyPin 4
#define SLEEP_ENABLED true // Je nach Modul kommt es nicht mehr zurück aus dem Sleep!

// MFRC522
#define RST_PIN 9
#define SS_PIN 10

// Buttons
#define buttonPause A0
#define buttonUp A1
#define buttonDown A2
#define LONG_PRESS 1000

// Max Volume
byte maxVolume = 30;

// Serial Baud
#define SERIAL_BAUD 115200

//------------------------------------
// StateMachine conditions
state_pg globalState = NORMAL;
state_p playerState = PAUSE;

// Button conditions
state_b pauseButtonState = RELEASED;
state_b upButtonState = RELEASED;
state_b downButtonState = RELEASED;

// MP3 Modus
playback_mode playmode = SINGLE;

//Variables
static uint16_t _lastTrackFinished;
static uint16_t numTracksInFolder;
static uint16_t currentTrack;

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
  SetStates();
  StateMachineGlobal();
}
