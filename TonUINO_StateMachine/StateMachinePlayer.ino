
void StateMachinePlayer() {
  
  switch(playerState) {
    
    case PAUSE: //pause
      if(b1State == PRESSED) {
        //mp3.play();
      } else if (b1State == LONGPRESSED) {
        //mp3.stop();

        // neue RFID Karte
        globalState = RFIDCARD;
      }
    break;

    case PLAYING: // playing
      //mp3.loop();
      
      if(b1State == PRESSED) {
        //mp3.pause();
      } else if (b1State == LONGPRESSED) {
        //Tracknummer ansagen
      }

      if(b2State == PRESSED) {
        playerState = NEXT_TRACK;
      }

      if(b3State == PRESSED) {
        playerState = PREVIOUS_TRACK;
      }
    break;

    case NEXT_TRACK:
      //mp3.next();
      playerState = PLAYING;
    break;

    case PREVIOUS_TRACK:
      playerState = PLAYING;
    break;
  }  
}
