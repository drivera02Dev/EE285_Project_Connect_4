#include <stdio.h>
#include "Connect4.h"


//int PORAI = 0; //sets whether playing PVP or AI
//THIS LINK BELOW IS KNOWN TO DISPLAY EVERYTHING PROPERLY
//no idea why tranfserring to this project casuses errors -- repl being dumb or something
//https://replit.com/join/unufwdtgmp-apmiller 


int main(void) {
  
  cursesStart();
  drawMenu();

  struct game_state game_state = {0};
  gameInit(&game_state);

  int ch = 0;//input character
  while(1){
    update(&game_state, ch);
    reDraw(&game_state);
    //comment below out if repl isn't liking it
    drawGameDetails(&game_state);
    if(game_state.AI == 1 && game_state.turn == P1){
      ch = 10;
      continue;//doesn't ask for char input if AI turn
  }
      ch = getch();
  }

  cursesEnd();

  return 0;
}