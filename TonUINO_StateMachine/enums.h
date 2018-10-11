typedef enum PlayerGlobalState {
  NORMAL = 1,
  RFIDCARD = 2  
} state_pg;

typedef enum PlayerState {
  PAUSE = 1,
  PLAYING = 2,
  NEXT_TRACK = 3,
  PREVIOUS_TRACK = 4
} state_p;

typedef enum ButtonState {
  RELEASED = 1,
  PRESSED = 2,  
  LONGPRESSED = 3
} state_b;

typedef enum PlaybackMode {
  AUDIODRAMA = 1, //Hörspielmodus
  FOLDER = 2, //Albummodus
  PARTY = 3, //Partymodus
  SINGLE = 4, // Einzelmodus
  AUDIOBOOK = 5  
} playback_mode;
