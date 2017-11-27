/*
 * proyectoFinalSnake.c
 *
 * Created: 11/27/2017 5:02:44 PM
 * Coauthor: Gustavo Gutiérrez Gómez
 * Coauthor: Eduardo Tavarez Quezada
 */

#include <mega328P.h>
#include <delay.h>
#include "matriz.h"

void main(void) {
    ConfiguraMax();
    while(1) {   
        MandaMax7219(0x0102);
    }
}