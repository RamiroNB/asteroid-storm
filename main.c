/* Nokia 5110 LCD AVR Library example
 *
 * Copyright (C) 2015 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 *
 * Original library written by SkewPL, http://skew.tk
 * Custom char code by Marcelo Cohen - 2021
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "nokia5110.h"

// fazer as interrupçoes
// fazer uma funçao para alterar a posiçao do boneco para nao ter q repetir muuti codigo
// fazer os asteroides
#define TIMER_CLK (F_CPU / 1024)

// sempre 2 pra esquerda e 2 pra cima
uint8_t ship_pos[2] = {};

uint8_t ship[] = {0b00010000, 0b00010000, 0b00111000, 0b01111100, 0b01010100, 0b00111000, 0b00010000, 0b00010000};

uint8_t asteroid_draw[] = {0b00011000,
                           0b00101100,
                           0b11111111,
                           0b00101100,
                           0b00011000};

// set timer interrupt for movind the space ship
int main(void)
{
    nokia_lcd_init();
    nokia_lcd_clear();
    nokia_lcd_set_cursor(80, 24);
    nokia_lcd_custom(1, ship);
    // nokia_lcd_write_string("IT'S WORKING!", 1);
    nokia_lcd_set_cursor(70, 0);
    nokia_lcd_write_char(1, 2);

    // nokia_lcd_write_string("Nice!\001", 2);
    // nokia_lcd_drawcircle(20, 20, 20);
    nokia_lcd_render();
    while (1)
        ;
    // while (1)
    // {
    //     nokia_lcd_clear();
    //     // for (int i = 0; i < 20; i++)
    //     // {
    //     //     uint8_t x1, y1, x2, y2;
    //     //     x1 = rand() % 84;
    //     //     y1 = rand() % 48;
    //     //     x2 = rand() % 84;
    //     //     y2 = rand() % 48;
    //     //     // uint8_t r = rand() % 10;
    //     //     nokia_lcd_drawline(x1, y1, x2, y2);
    //     //     // nokia_lcd_drawrect(x1, y1, x2, y2);
    //     //     // nokia_lcd_drawcircle(x1, y1, r);
    //     // }
    //     nokia_lcd_render();
    //     // _delay_ms(100);
    // }
}
