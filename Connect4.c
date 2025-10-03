#include "Connect4.h"

int PORAI =0;

//cureses intialization
void cursesStart(){
  initscr();
  clear();
  noecho();
  cbreak();
  curs_set(0);
  keypad(stdscr, true);
  //centerX = COLS/2;
  //centerY = LINES/2;

  if(LINES<25 && COLS<50){//screen size check
    endwin();
    printf("Your terminal is too small\n");
    exit(1);
  }

  if(!has_colors()) {	//color check
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }
  start_color();
  init_pair(P1, C_P1, COLOR_RED);
  init_pair(P2, C_P2, COLOR_YELLOW);
  init_pair(BOARD, C_BOARD, COLOR_BLUE);
}

//curses quit
void cursesEnd(){
  curs_set(1);
  endwin();
 }

void drawGameDetails(struct game_state *game_state){
  WINDOW *inGameMenu = newwin(15, 50, 10, 25);

  box(inGameMenu, 0, 0);
  mvwprintw(inGameMenu, 1, 1, "Welcome to Connect4!");
  mvwprintw(inGameMenu, 2, 1, "Use the Left and Right Arrow Keys to move");
  mvwprintw(inGameMenu, 3, 1, "Use the Down Arrow to Place a Piece");
  mvwprintw(inGameMenu, 4, 1, "Player %d's turn!", game_state->turn);
  mvwprintw(inGameMenu, 7, 1, "Press Space to Reset board");
  unsigned char player = game_state->turn;
  drawPiece(40, 3, 1, 5, player);
    if(game_state->winner != NON_P){
      mvwprintw(inGameMenu, 10, 1, "Player %d wins!", game_state->winner);
    }

  wrefresh(inGameMenu);
}
//displaying the menu
void drawMenu(){

  WINDOW *menu_win = newwin(15, 40, (LINES - GAMEROW * BOXH) / 2, (COLS - GAMECOL * BOXW) / 2);

  keypad(menu_win, true); 

  box(menu_win, '|', '-'); 
  refresh(); 
  mvwprintw(menu_win, 1, 1, "Welcome to Connect4!");
  mvwprintw(menu_win, 2, 1, "Use the UP/DOWN Arrow Keys to Select");
  mvwprintw(menu_win, 3, 1, "Press Enter to Begin Game");

  wrefresh(menu_win);
  char *choices[2] = {"Player Vs. Player", "Player vs. AI"};
  int choice;
  int highlight = 0;
  while(1){
    for(int i=0; i<2; i++){
      if(i==highlight){
        wattron(menu_win, A_REVERSE);
      }
        mvwprintw(menu_win, i+10, 1, choices[i]);
        wattroff(menu_win, A_REVERSE);
    }
      choice = wgetch(menu_win);

      switch(choice){

        case KEY_UP:
          highlight--;
          if(highlight == -1){
            highlight = 0;
          }
          break;

        case KEY_DOWN:
          highlight++;
          if(highlight==2){
            highlight=1;
          }
          break;

        default:
          break;
      }
      if(choice == 10){//if enter key
        if(highlight == 1){
          PORAI = 1;
        }
        wattroff(menu_win, A_REVERSE);
        break;
      }
    //printw("your choice was %s",choices[highlight]);
    //refresh();
  }
}

//draw pieces at specified coords with player for color
void drawPiece(int startX, int startY, int posX, int posY, unsigned char player){
  int x = posX * BOXW + 1;
  int y = posY * BOXH + 1;

  attron(COLOR_PAIR(player));

  //create actual pieces with these
  mvaddch(startY +y, startX +x, ACS_BOARD); 
  mvaddch(startY + y, startX +x+1, ACS_BOARD);

  attroff(COLOR_PAIR(player));
}

//draws current pieces on the board
void drawPieces(int startX, int startY, unsigned char *board){

  for(int i=0; i<GAMEROW; i++){
    for(int j=0; j<GAMECOL; j++){
      unsigned char player = board[i*GAMECOL +j];
      if(player != NON_P){
        drawPiece(startX, startY, j, i, player);
      }
    }
  }

}

//draw the board, borders and all
void drawBoard(int startX, int startY){

  int endY = startY + GAMEROW * BOXH;
  int endX = startX + GAMECOL * BOXW;

  attron(COLOR_PAIR(BOARD)); // set color of board for output

  //Hoirzontal lines
  for(int i = startY; i<= endY; i += BOXH){
    for(int j = startX; j <= endX; j++){
      mvaddch(i, j, ACS_HLINE);
    }
  }
  //Vertical Lines
  for(int i=startX; i<= endX; i+=BOXW){
    for(int j=startY; j<=endY; j++){
      mvaddch(j,i, ACS_VLINE);
    }
  }

  for(int i= startX + BOXW; i<= endX - BOXW; i +=BOXW){
    mvaddch(startY, i, ACS_TTEE);
    mvaddch(endY, i, ACS_BTEE);
  }

  for(int i= startY + BOXH; i<= endY - BOXH; i+=BOXH){
    mvaddch(i, startX, ACS_LTEE);
    mvaddch(i, endX, ACS_RTEE);
  }

  mvaddch(startY, startX, ACS_ULCORNER);
  mvaddch(startY, endX, ACS_URCORNER);
  mvaddch(endY, startX, ACS_LLCORNER);
  mvaddch(endY, endX, ACS_LRCORNER);

  attroff(COLOR_PAIR(BOARD));

}
//draws whoevers turn player piece above board
void drawPossible(int startX, int startY, int pos, unsigned char player){
  drawPiece(startX, startY, pos, -1, player);
}

//put piece on next available spot in specified column
bool placePiece(unsigned char *board, int pos, unsigned char player){
  for(int i=5; i>=0; i--){
    if(board[i*GAMECOL + pos] == 0){
      board[i* GAMECOL +pos] =player;
      return true;
    }
  }
  return false;
}

//check 4 in a rows in all directions
unsigned char checkFor4(int startX, int startY, unsigned char *board, int dirX, int dirY){
  unsigned char player = board[startY * GAMECOL + startX];

  for(int i=1; i<4; i++){
    if(startX + i * dirX >= GAMECOL || startX + i * dirX <0){
      return NON_P;
    } else if(startY + i * dirY >= GAMEROW || startY + i * dirY < 0){
      return NON_P;
    } else if (board[(startY + i * dirY) * GAMECOL + (startX +i *dirX)] != player){
      return NON_P;
    }
  }
  return player;
}

//checks if 4 in row returns anything relating to a winning player
unsigned char checkWin(unsigned char *board){
  unsigned char player = NON_P;

  for(int i=0; i<GAMEROW; i++){
    for(int j=0; j<GAMECOL; j++){
      if(board[i * GAMECOL + j] == NON_P){
        continue;
      }
      //sum up all possible 4 in a rows, assign player to 
      player = checkFor4(j, i, board, 1, 0);
      player += checkFor4(j, i, board, 0, 1);
      player += checkFor4(j, i, board, 1, 1);
      player += checkFor4(j, i, board, 1, -1);

      if(player != NON_P){
        return player;
      }
    }
  }
  return NON_P;
}

//intializes game, sets game_state vars and allocates board
void gameInit(struct game_state *game_state){

  if(game_state->board == NULL){
    //allocate empty board array
    game_state->board = calloc(GAMECOL *GAMEROW, sizeof(unsigned char));
  } else 
    memset(game_state->board, 0, GAMECOL * GAMEROW);

  if(game_state->board == NULL){
    endwin();
    printf("error allocating board");
    exit(1);
  }

  game_state->turn = P1;
  game_state->winner = NON_P;
  game_state->player_pos = 0;
  game_state->AI = PORAI;

}

float alphabetaFunc(int node, float depth, float alpha, float beta, bool maximizingPlayer, unsigned char *board) {

  float best;
  if (depth == 3)
    return *board;

  if (maximizingPlayer) {

     best = MIN;

    for (int i = 0; i < 2; ++i) {

      float val = alphabetaFunc(node*2+i, depth+1, alpha, beta, FALSE, board);

      best = fmax(best, val);
      alpha = fmax(alpha, best);

      if (beta <= alpha)
        break;

    }
    return best;
  }

  else {

    best = MAX;

    for (int i = 0; i < 2; ++i) {

      float val = alphabetaFunc(node*2+i, depth+1, alpha, beta, TRUE, board);

      best = fmin(best, val);
      beta = fmin(beta, best);

      if (beta <= alpha) 
        break;

    }
    return best;
  }
}

void update(struct game_state *game_state, int ch){
  //need something in here along the lines of if(PORAI == 1){ on P2 turn, choose best place and placepiece}
  int spot;
  int node = 6;
  float depth = 1;
  float alpha = 50;
  float beta = 50;
  bool maximizingPlayer = FALSE;

  if (game_state->AI == 1 && game_state->turn == P1 && game_state->winner == NON_P) {
    //exit(1);
    /*mvprintw(0, 0, "If statement entered");
    refresh();*/
    sleep(1);
    //sets position to best column from function
    game_state->player_pos = alphabetaFunc(node, depth, alpha, beta, maximizingPlayer, game_state->board);

    mvprintw(1,0, "player_pos is: %d", game_state->player_pos);

    spot = placePiece(game_state->board, game_state->player_pos, game_state->turn);
    /*mvprintw(2,0, "spot is: %d", spot);
    refresh();
    sleep(2);*/

    if(spot){//update who's turn it is
      //game_state->turn = (game_state->turn == P1) ? P2 : P1;
      if(game_state->turn == P1){
        game_state->turn = P2;
      } else {
      game_state->turn = P1;
      } 
    }
  }
  //arrow key movements, only allows moving inbounds
  unsigned char winPlayer;
  if(ch == KEY_LEFT && game_state->player_pos > 0){
    game_state->player_pos--;//move left
  } else if(ch == KEY_RIGHT && game_state->player_pos < 6){
    game_state->player_pos++;//move right
  } else if(game_state->winner == NON_P && ch == KEY_DOWN){
    spot = placePiece(game_state->board, game_state->player_pos, game_state->turn);

      if(spot){//update who's turn it is
        //game_state->turn = (game_state->turn == P1) ? P2 : P1;
        if(game_state->turn == P1){
          game_state->turn = P2;
        } else {
        game_state->turn = P1;
        } 
      }
  } else if(ch == ' '){
    gameInit(game_state);
  }

  winPlayer = checkWin(game_state->board);
  if(winPlayer != NON_P){
    game_state->winner = winPlayer;
  }

}

void reDraw(struct game_state *game_state){

  int startY = (LINES - GAMEROW * BOXH) / 2;
  int startX = (COLS - GAMECOL * BOXW) / 2;

  clear();

  drawPossible(startX, startY, game_state->player_pos, game_state->turn);
  drawBoard(startX, startY);
  drawPieces(startX, startY, game_state->board);

  /*if(game_state->winner != NON_P){
    mvprintw(LINES-2, 1, "Player %d wins", game_state->winner);
  }*/
  refresh();

}