/*
 * proyectoFinalSnake->c
 *
 * Created: 11/27/2017 5:02:44 PM
 * Coauthor: Gustavo Guti�rrez G�mez
 * Coauthor: Eduardo Tavarez Quezada
 */
             
 #asm
   .equ __lcd_port=0x05
   .equ __lcd_EN=1
   .equ __lcd_RS=0
   .equ __lcd_D4=2
   .equ __lcd_D5=3
   .equ __lcd_D6=4
   .equ __lcd_D7=5
#endasm
 
#include <mega328P.h>
#include <display.h>
#include <delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "matriz.h"

// Direction constants
const signed char UP = 0;
const signed char RIGHT = 1;
const signed char DOWN = 2;
const signed char LEFT = 3;
// Max length constant
const signed char MAX_LENGTH_SNAKE = 24;

// Game variables
unsigned char direction;
signed char snakeX[MAX_LENGTH_SNAKE]; // snakeX[0] is head
signed char snakeY[MAX_LENGTH_SNAKE]; // snakeY[0] is head
unsigned char snakeLength;
unsigned char matrix[8][8];

signed char pelletX;
signed char pelletY;

signed char points;
signed int speed;
signed char gameRunning;

// Iterator variables
int i;
int j;

// Matrix variables
unsigned int mX;
unsigned int mY;
unsigned int m;

// Functions
void clearMax() {
    MandaMax7219(0x0800);
    MandaMax7219(0x0700);
    MandaMax7219(0x0600);
    MandaMax7219(0x0500);
    MandaMax7219(0x0400);
    MandaMax7219(0x0300);
    MandaMax7219(0x0200);
    MandaMax7219(0x0100);
}

void endGame() {
    EraseLCD();
    StringLCD("PERDISTE!");
    MoveCursor(0, 1);
    StringLCD("Pica ");
    CharLCD(94);
    StringLCD(" para jugar");
    while (PIND.0);
    gameRunning = 0;
}

void moveSnake() {
    for (i = snakeLength - 1; i > 0; i--) {
    //     // if ((direction == RIGHT) && (snakeX[0]-snakeLength == 7)) {
    //     //     snakeX[0] = -1;
    //     // } else if ((direction == LEFT) && (snakeX[0]+snakeLength == 0)) {
    //     //     snakeX[0] = 8;
    //     // } else {
            snakeX[i] = snakeX[i-1];
    //     // }

    //     // if ((direction == UP) && (snakeY[0]+snakeLength == 0)) {
    //     //     snakeY[0] = 8;
    //     // } else if ((direction == DOWN) && (snakeY[0]-snakeLength == 7)) {
    //     //     snakeY[0] = -1;
    //     // } else {
            snakeY[i] = snakeY[i-1];
    //     // }
    }
    switch (direction) {
        case UP: {
            snakeY[0] = snakeY[0] + 1;
            if (snakeY[0] > 7) {
                snakeY[0] = 0;
            }
            break;
        }
        case RIGHT: {
            snakeX[0] = snakeX[0] - 1;
            if (snakeX[0] < 0) {
                snakeX[0] = 7;
            }
            break;
        }
        case DOWN: {
            snakeY[0] = snakeY[0] - 1;
            if (snakeY[0] < 0) {
                snakeY[0] = 7; 
            }
            break;
        }
        case LEFT: {
            snakeX[0] = snakeX[0] + 1;
            if (snakeX[0] > 7) {
                snakeX[0] = 0;
            }
            break;
        }
    }
}

void printBoard() {
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            matrix[i][j] = 0;
        }
    }
    clearMax();
    for (i = 0; i < snakeLength; i++) {
        matrix[snakeX[i]][snakeY[i]] = 1;
    }
    matrix[pelletX][pelletY] = 1;

    mX = 256;
    for (i = 0; i < 8; i++) {
        mY = 0;
        if (matrix[i][7]) {
            mY++;
        }
        for (j = 6; j >= 0; j--) {
            mY = mY << 1;
            if (matrix[i][j]) {
                mY++;
            }
        }
        m = mX + mY;
        MandaMax7219(m);
        mX = mX + 256;
    }
}

// void growSnake() {
//     //declara el nuevo final de la lista
//     tail->prev = new_node;
//     new_node->next = tail;
//     new_node->prev = NULL;
//     new_node->position = lastPos;
//     tail = new_node;
// }

char isCollinding(signed char x, signed char y) {
    for (i = 0; i < snakeLength - 1; i++) {
        if ((x == snakeX[i]) && (y == snakeY[i])) {
            return 1;
        }
    }
    return 0;
}

void setPellet(){
    do {
        pelletX = rand()%8;
        pelletY = rand()%8;
    } while (isCollinding(pelletX, pelletY));
}

void increaseSpeed() {
    if (points > 15) {
        speed = 100;
    } else {
        speed = 500 - (points * 25);
    }
}

void doEatPellet() {
    if ((snakeX[0] == pelletX) && (snakeY[0] == pelletY)) {
        snakeLength++;
        points++;
        setPellet();
        increaseSpeed();
    }
}

void doEatSnake() {
    for (i = 1; i < snakeLength; i++) {
        if ((snakeX[0] == snakeX[i]) && (snakeY[0] == snakeY[i])) {
            endGame();
        }
    }
}

void checkButtons() {
    if (PIND.0 == 0) {
        direction = UP;
    } else if (PIND.1 == 0) {
        direction = RIGHT;
    } else if (PIND.2 == 0) {
        direction = DOWN;
    } else if (PIND.3 == 0) {
        direction = LEFT;
    }
}

void main(void) {
    // Pull-up from PD0 to PD3 (BUTTONS)
    PORTD = 0x0F;

    // Start-up timer 0B in CK/1
    TCCR0B = 0x01;

    // Configures Matrix and LCD
    ConfiguraMax();
    SetupLCD();

    // Create snake position arrays
    snakeX[0] = 7;
    snakeY[0] = 7;
    snakeLength = 1;
    for (i = 3; i < MAX_LENGTH_SNAKE; i++) {
        snakeX[i] = -1;
        snakeY[i] = -1;
    }

    // Set up game variables
    points = 0;
    speed = 500;
    direction = RIGHT;
    gameRunning = 1;

    // Set up points in LCD
    StringLCD("Puntos: ");
    MoveCursor(8,0);
    CharLCD(points+48);   
    MoveCursor(8,0);

    // Set up random seed
    srand(TCNT0);

    setPellet();

    while (1) {
        while (gameRunning) {
            checkButtons();
            moveSnake();
            doEatPellet();
            doEatSnake();

            clearMax();
            printBoard();
            delay_ms(speed);
        }
        EraseLCD();
        StringLCD("Esto ya paro");
    }
}