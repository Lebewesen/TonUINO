void StateMachineGlobal() {
  
  switch(globalState) {
    
    case NORMAL:
      StateMachinePlayer();
    break;

    case RFIDCARD:
      CardHandling();
    break;
  }
}
