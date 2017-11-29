/*
 * proyectoFinalSnake.c
 *
 * Created: 11/27/2017 5:02:44 PM
 * Coauthor: Gustavo Gutiérrez Gómez
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


typedef struct node {
    int position;
    struct node* next;
    struct node* prev;
}node;

signed char snakePos;
signed char pelletPos;
unsigned char snakeLen;
unsigned char points;
unsigned char direction;
unsigned int speed;
unsigned char pelletCatched=1;

node* head;
node* tail;




unsigned int dots[64] = {
    0x0880, 0x0780, 0x0680, 0x0580, 0x0480, 0x0380, 0x0280, 0x0180,
    0x0840, 0x0740, 0x0640, 0x0540, 0x0440, 0x0340, 0x0240, 0x0140,
    0x0820, 0x0720, 0x0620, 0x0520, 0x0420, 0x0320, 0x0220, 0x0120,
    0x0810, 0x0710, 0x0610, 0x0510, 0x0410, 0x0310, 0x0210, 0x0110,
    0x0808, 0x0708, 0x0608, 0x0508, 0x0408, 0x0308, 0x0208, 0x0108,
    0x0804, 0x0704, 0x0604, 0x0504, 0x0404, 0x0304, 0x0204, 0x0104,
    0x0802, 0x0702, 0x0602, 0x0502, 0x0402, 0x0302, 0x0202, 0x0102,
    0x0801, 0x0701, 0x0601, 0x0501, 0x0401, 0x0301, 0x0201, 0x0101
};

void addpellet(node* last_node, int position)
{
    //declara la nueva cola
    node* new_node = (struct node *)malloc(sizeof(struct node));
    //declara el nuevo final de la lista
    last_node->prev = new_node;
    new_node->next = last_node;          
    new_node->prev = NULL;
    new_node->position = position;
    tail = new_node;

}


void printsnake(node* head)
{
    node* cursor = head;
    while(cursor != NULL)
    {
        cursor = cursor->prev;
    }
}

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

void setPellet(){
    while((pelletPos = rand()%64) == snakePos);
}

unsigned char doCatchPellet() {
    if (snakePos == pelletPos) {
        points++;
        if (points > 15) {
            speed = 100;
        } else {
            speed = 500 - (points * 25);
        }
        return 1;
    }
    return 0;
}

void main(void) {
    
    srand(1);
    ConfiguraMax();
    SetupLCD();
    
    PORTD = 0x0F;        // Pull-up de PD0 a PD3
    snakePos = 19;
    snakeLen = 1;
    points = 0;
    direction = 1;
    speed = 500;
    StringLCD("Puntos: ");
    MoveCursor(8,0);
    while(1) {    
        // up       
        CharLCD(points+48);   
        MoveCursor(8,0);
        if (PIND.0 == 0) {
            direction = 0;
        } else if (PIND.1 == 0) {
            direction = 1;
        } else if (PIND.2 == 0) {
            direction = 2;
        } else if (PIND.3 == 0) {
            direction = 3;
        }
        
        switch (direction) {
            case 0: {
                snakePos -= 8;
                if (snakePos < 0) {  
                    snakePos += 64;
                }
                break;
            }
            case 1: {
                snakePos += 1;
                if (snakePos%8 == 0) {
                    snakePos -= 8;
                }
                break;
            }
            case 2: {     
                snakePos += 8;
                if (snakePos > 63) {  
                    snakePos -= 64;
                }
                break;
            }
            case 3: {         
                snakePos -= 1;
                if (snakePos%8 == 7 || snakePos == -1) {
                    snakePos += 8;
                }
                break;
            }
        }
        
        pelletCatched = doCatchPellet();
        if(pelletCatched) {
            setPellet();
            pelletCatched = 0;           
        }
        MandaMax7219(dots[snakePos]);  
        MandaMax7219(dots[pelletPos]);
        
        delay_ms(speed);
        clearMax();
    }
}