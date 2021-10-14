#include "menu.h"
#include "glcd.h"
#include "uart.h"

#define BOARD_ROWS 6
#define BOARD_COLS 7

int tempx;       // temporary values of ADC
int tempy;       // temporary values of ADC
int pressed;     // variable used in function to detect pressure on GLCD touch screen
int resultx;     // variable to store our ADC result
int resulty;     // variable to store our ADC result
int choice;      // variable used to return player choice of collumn (player_choice)
int mode;        // variable used to return game mode choice (mode_choice)
int row, col, idx, count;   // variables used to to check winner (check_ functions)

void opening_menu(){
    
    glcd_SetCursor(25,0);                                           //
    glcd_WriteString("Connect 4", f8X8, 2);                         //
    glcd_SetCursor(22,4);                                         //  Opening Screen
    glcd_WriteString("Start Game", f8X8, 2);                   //
    //glcd_SetCursor(15,6);                                           //
    //glcd_WriteString("Start Game", f8X8, 2);                        //
    __delay_ms(1000);                                               //

// single player mode not available

        mode = mode_choice();       // call mode_choice, returns 1 if multi-player mode is chosen
        
        if (mode){                  // if multi-player is chosen, print a line under it (visual cue) and call menu_game
                glcd_SetCursor(22,5);
                glcd_WriteString("----------", f8X8, 2);
                __delay_ms(250);
                glcd_FillScreen(0);      
                menu_game();}
}
        //}}}

void menu_game(){
    
unsigned char board[42];        // create matrix that represents the entire game board (42 spots)
const char *PIECES = "XO";      // store the game pieces (to be printed inside the board)

for (int i=0;i<42;i++) {   // Filling the array with white spaces (a white space means there is no pieces yet in this specific location)
    board[i]= ' ';}

int turn = 0;
int done = 0;


   for(turn = 0; turn < BOARD_ROWS * BOARD_COLS && !done; turn++){         // game loop : repeat while "turn" is less than 42 (which is the max number of positions on board) and "done" is still 0
      printBoard(board);                                                   // call print board function that prints the board with the latest changes
      
      // Check "turn" reminder when divided by 2: if 0 then turn is even (player 1 turn),  if 1 then turn is odd (player 2 turn)
      switch (turn%2){                                            
            case 0:                                                 
            glcd_SetCursor(10,0);                                   
            glcd_WriteString("Player 1 turn", f8X8, 2);
            break;
          case 1:
            glcd_SetCursor(10,0);
            glcd_WriteString("Player 2 turn", f8X8, 2);
            break;
      }
   
      __delay_ms(250);   // 

      
      // We enter this while loop if takeTurn returns 0: which means that the column choice is not valid (column full)
      while(!takeTurn(board, turn % 2, PIECES)){   // Even if we don't enter the loop takeTurn is executed (so the 'board' is filled anyway)
        printBoard(board);                        
        glcd_SetCursor(10,1);
        glcd_WriteString(" Collumn full !", f8X8, 2);
         }
        glcd_SetCursor(0,1);
        glcd_WriteString("                ", f8X8, 2);  // clear the player turn message, it will be filled again if game is not done
        
        done = checkWin(board);                         // call the function checkWin, it return 1 if there is a winner. And we also get out of the for loop: game finished.
   }

 printBoard(board);                                     // print updated board
 
if(turn == BOARD_ROWS * BOARD_COLS && !done){           // if we reached 42 turn and there is still no winner, we print "it's a tie"
      glcd_SetCursor(0,1);
      glcd_WriteString("                ", f8X8, 2);
      glcd_SetCursor(0,1);
      glcd_WriteString("It's a tie!", f8X8, 2);
              
   } else {                                             // else means we are still below 42 rounds  
      turn--;                                           // "turn" is incremented even after win, so we get it back to the actual winning turn  
            switch (turn%2+1){                          // switch case: if turn is even 
            case 1: 
      glcd_SetCursor(0,0);
      glcd_WriteString("        ", f8X8, 2);            // HOW TO ERASE EXACTLY 1 LINE
      glcd_SetCursor(1,0);
      unsigned char message1[] = "Player 1(X) wins";
      glcd_WriteString(message1, f8X8, 2);
      uartWrite('X');
      __delay_ms(2000);
      break;
          case 2:
      glcd_SetCursor(0,0);
      glcd_WriteString("        ", f8X8, 2);
      glcd_SetCursor(1,0);
      unsigned char message2[] = "Player 2(0) wins";
      glcd_WriteString(message2, f8X8, 2);
      uartWrite('O');
      __delay_ms(2000);
      //glcd_FillScreen(0);
      break;
   }

   //return 0;
}
}

// Print the game board from the "board" matrix filled with pieces (X/O)
void printBoard(char *board){
       for (int row=0; row<6; row++){                       // Range of Values: x[0-128] & y[0-7]
          for (int col=0; col<7; col++){ 
            glcd_SetCursor(2+col*17,7-row);                 // set cursor on lower-left side. and each time we move column we add 17 pixels to the x-axis
            glcd_WriteString("I", f8X8, 2);                 // print the walls of the board (using the letter i)
            //char temp = check_PORTA();
            char temp = board[BOARD_COLS * row + col];      // store the content of the "board" matrix at this particular position
            glcd_WriteChar8X8(temp, 2);                     // after each "wall" we print the content of the "board" matrix at this particular position. it can be either: 'X'/'O'/' ' 
          }
    glcd_WriteString("I", f8X8, 2);                         // this wall represents the last column on the right side 

   }
}


int takeTurn(char *board, int player, const char *PIECES){
   int row, col;
   col = player_choice();   // call player_choice function, it returns the the column number chosen
   col--;  // b/c board matrix begins at 0

   for(row = 0; row <6; row++){
      if(board[BOARD_COLS * row + col] == ' '){          // Check if spot in 'board' is free. begin from lower row in chosen column and move up. if there is already a piece there, don't enter the loop 
        board[BOARD_COLS * row + col] = PIECES[player];  // fill position in board 
        return 1;                                        // return 1 to say that position is filled
               
      }
   }
//}
   return 0; // Position in board CANNOT be filled and we exit function takeTurn()
}


int player_choice(){

    // initialize needed variables
    choice = 0;
    tempx = 0;
    tempy = 0;
    pressed = 0;

    do {   //
        __delay_us(5);  //Wait the acquisition time (about 5us).
        
        // Get X postion
        ADCON0bits.CHS = 0;             // select analog input, AN0
        ADCON0bits.GO = 1;              // start the conversion 
        PORTCbits.RC0 = 1;              // set RC0 = 1. This is needed to get values from AN0
        
        while(ADCON0bits.GO==1){};      // wait for the conversion to end 
        resultx = (ADRESH<<8)+ADRESL;	// combine the 10 bits of the conversion
        PORTCbits.RC0 = 0;              

        __delay_us(5);  
        
        // Get Y position
        ADCON0bits.CHS = 1;             // select analog input, AN1
        ADCON0bits.GO = 1;              // start the conversion 
        PORTCbits.RC1 = 1;              // set RC1 = 1. This is needed to get values from AN1
        
        while(ADCON0bits.GO==1){};      // wait for the conversion to end 
        resulty = (ADRESH<<8)+ADRESL;	// combine the 10 bits of the conversion
        PORTCbits.RC1 = 0;              // set RC1 back to 0
        
        // this loop checks the position of your finger on the touch screen        
        if (((resultx == tempx) && (resulty == tempy)) && ((tempx > 350) && ((tempy < 896)))){  // We enter the loop if : the x&y coordinates are still the same as the loop before, which means we
                                                                                                // reached (and maintained at least 250ms) the voltage associated with the current position. 
                                                                                                // Also if the x&y are higher/lower than their "at rest" values (320 and 896 respectively)
        // Each statement corresponds to x&y coordinates of each column on the board            
            if ( ((resultx == 384) || (resultx == 448)) && (resulty == 832))                         
        {
            choice = 1;
            pressed = 1;
            break;
        }
       
        
        if ( ((resultx == 512) && (resulty == 768)) || ((resultx == 576) && (resulty == 704)) || ((resultx == 576) && (resulty == 704)) )
        {    
            choice = 2;
            pressed = 1;
            break;
        }
        
        if ( ((resultx == 576) && (resulty == 768)) || ((resultx == 640) && (resulty == 704)) || ((resultx == 704) && (resulty == 640)) )
        {    
            choice = 3;
            pressed = 1;
            break;
        }
        
        if ( ((resultx == 640) && (resulty == 768)) || ((resultx == 704) && (resulty == 704)) || ((resultx == 768) && (resulty == 704)) || ((resultx == 768) && (resulty == 640)) )
        {    
            choice = 4;
            pressed = 1;
            break;
        }
        
        if ( ((resultx == 704) && (resulty == 768)) || ((resultx == 768) && (resulty == 768)) || ((resultx == 832) && (resulty == 704)) )
        {   
            choice = 5;
            pressed = 1;
            break;
        }
        
        if ( ((resultx == 832) && (resulty == 768)) || (( (resultx == 960) || (resultx == 896)) && (resulty == 704)) )
        {    
            choice = 6;
            pressed = 1;
            break;
        }
        
        if ( (((resultx == 896) || (resultx == 960) || (resultx == 1024)) && (resulty == 768)) || ((resultx == 1152) && (resulty == 704)) )
        {    
            choice = 7;
            pressed = 1;
            break;
        }
            
        }
        
         __delay_ms(250);       // this delay is used to check the ADC results at 2 different intervals.
         
        tempx = resultx;        // we store a temporary value of x&y to compare them afterwards. 
        tempy = resulty;
        
        
    } while(pressed != 1);      // exit the loop when a position is chosen

    return choice;              // return the choice of the player (1 to 7)
}

// Same as player_choice but applied to the opening menu
int mode_choice(){
  
    tempx = 0;
    tempy = 0;
    pressed = 0;
    
    do {   //
        __delay_us(5);                  // Wait the acquisition time (about 5us).
        
        ADCON0bits.CHS = 0;             // select analog input, AN0
        ADCON0bits.GO = 1;              // start the conversion
        PORTCbits.RC0 = 1;              // set RC0 = 1. This is needed to get values from AN0
        
        while(ADCON0bits.GO==1){};      // wait for the conversion to end 
        resultx = (ADRESH<<8)+ADRESL;	// combine the 10 bits of the conversion
        PORTCbits.RC0 = 0;              // set RC0 back to 0

        __delay_us(5);  
        
        ADCON0bits.CHS = 1;             // select analog input, AN1
        ADCON0bits.GO = 1;              // start the conversion
        PORTCbits.RC1 = 1;              // set RC1 = 1. This is needed to get values from AN1
        
        while(ADCON0bits.GO==1){};      // wait for the conversion to end 
        resulty = (ADRESH<<8)+ADRESL;	// combine the 10 bits of the conversion
        PORTCbits.RC1 = 0;              // set RC1 back to 0
        
        // same as above, wait for stabilized voltage and when out of "rest mode"
        if (((resultx == tempx) && (resulty == tempy)) && ((tempx > 350) && ((tempy < 896)))){
        
        // Here we check if the player has pressed on the Multi-player mode
        if ( ((resultx == 512) || (resultx == 576) || (resultx == 704) || (resultx == 640) || (resultx == 768)) && ((resulty == 704) || (resulty == 640) || (resulty == 768)))
        {    
            pressed = 1;
            break;         
        }
        }
        
         __delay_ms(250);
         
        tempx = resultx;
        tempy = resulty;
        
        
    } while(pressed != 1);

    return pressed;
}


// Check if 1 of the 3 "checking" functions is true
int checkWin(char *board){
    return (horizontalCheck(board) || verticalCheck(board) || diagonalCheck(board));

}   

// Function that Returns TRUE (1) when all 4 checked boxes are equal and different from ' '
int checkFour(char *board, int a, int b, int c, int d){
    return (board[a] == board[b] && board[b] == board[c] && board[c] == board[d] && board[a] != ' ');

}

// Horizontal Check
int horizontalCheck(char *board){
    row, col, idx = 0;
    const int WIDTH = 1;
                                                                                            // Horizontal check: we loop in each row (a row corresponds to 7 consecutive spots in the array) 
    for(row = 0; row < BOARD_ROWS; row++){                                                  // and column (a column is a series of "position" separated by 7 elements in the array) and check the values of 4 consecutive spots
       for(col = 0; col < BOARD_COLS - 3; col++){                                           // in the matrix. If these 4 elements have the same "piece" stored in them, then we return 1.
          idx = BOARD_COLS * row + col;
          if(checkFour(board, idx, idx + WIDTH, idx + WIDTH * 2, idx + WIDTH * 3)){         // here WIDTH is equal to 1, we check the cells one after the other consecutively (unlike the other checking functions below) 
             return 1;
          }
       }
    }
    return 0;

}

int verticalCheck(char *board){
    row, col, idx = 0;
    const int HEIGHT = 7;

    for(row = 0; row < BOARD_ROWS - 3; row++){
       for(col = 0; col < BOARD_COLS; col++){
          idx = BOARD_COLS * row + col;                                                  // Vertical check: same principle, loop through all the spots in the matrix.
          if(checkFour(board, idx, idx + HEIGHT, idx + HEIGHT * 2, idx + HEIGHT * 3)){   // to move from row to row we add 7 (7 places in the matrix), this way we can compare number that are one above the other
              return 1;
          }
       }
    }
    return 0;

}

// Checks both direction and checks middle column twice for both direction
int diagonalCheck(char *board){
   row, col, idx, count = 0;
   const int DIAG_RGT = 6, DIAG_LFT = 8;

   for(row = 0; row < BOARD_ROWS - 3; row++){          // loop for the 4 lower rows only (b/c we can't do a 4 piece diagonal from over the 4th row)
      for(col = 0; col < BOARD_COLS; col++){
         idx = BOARD_COLS * row + col;
         if(count <= 3 && checkFour(board, idx, idx + DIAG_LFT, idx + DIAG_LFT * 2, idx + DIAG_LFT * 3) || count >= 3 && checkFour(board, idx, idx + DIAG_RGT, idx + DIAG_RGT * 2, idx + DIAG_RGT * 3)){
            return 1;                               // In the first 3 column (count <= 3), check for a diagonal to the right direction (we add DIAG_LEFT to the index, so that the next position we check is 8 spots after 
         }                                          // the current one). 8 spots means that the next number we check will be "above & to the right" of the index we are currently on.   
         count++;                                   // We do the same procedure for the last 3 column. This time we add 6 spots to the the current index. This way we are able to check the numbers that are  
      }                                             // "above and to the left" of the index we are currently on.
      count = 0;
   }
   return 0;

}


