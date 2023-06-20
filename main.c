#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "nokia5110.h"

// fazer as interrupçoes
// fazer uma funçao para alterar a posiçao do boneco para nao ter q repetir muuti codigo
// fazer os asteroides
// set timer interrupt for movind the space ship
#define TIMER_CLK (F_CPU / 1024)
#define MAX_TOP 0
#define MAX_BOTTON 74
#define MAX_RIGHT 0
#define MAX_LEFT 34

uint8_t ship[] = {0b00010000, 0b00010000, 0b00111000, 0b01111100, 0b01010100, 0b00111000, 0b00010000, 0b00010000};
uint8_t asteroid_draw[] = {0b00011000, 0b00101100, 0b00111100, 0b00110100, 0b00011000};

uint8_t asteroids[][2] = {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}}; 
uint8_t ship_pos[2] = {MAX_BOTTON, 15};

ISR(PCINT2_vect)
{
    if (!(PIND & (1 << PIND0)))
    {
        int current = ship_pos[0];
        if ((current - 5) >= MAX_TOP)
        {
            ship_pos[0] = current - 5;
            while (!(PIND & (1 << PD0)))
                _delay_ms(0.2); // debounce
        }
    }
    if (!(PIND & (1 << PIND2)))
    {
        if ((ship_pos[0] + 5) <= MAX_BOTTON)
        {
            int current = ship_pos[0];
            ship_pos[0] = current + 5;
            while (!(PIND & (1 << PD2)))
                _delay_ms(0.2); // debounce
        }
    }
    if (!(PIND & (1 << PIND1)))
    {
        if ((ship_pos[1] + 4) <= MAX_LEFT)
        {
            int current = ship_pos[1];
            ship_pos[1] = current + 4;
            while (!(PIND & (1 << PD1)))
                _delay_ms(0.2); // debounce
        }
    }
    if (!(PIND & (1 << PIND3)))
    {
        if ((ship_pos[1] - 4) >= MAX_RIGHT)
        {
            int current = ship_pos[1];
            ship_pos[1] = current - 4;
            while (!(PIND & (1 << PD3)))
                _delay_ms(0.2); // debounce
        }
    }
}
//timer de um seg
int count  = 0;
int count_ve = 0;
int count_create = 0;
int min_vel = 40;
int seg = 0;
ISR(TIMER1_COMPA_vect)
{   count++;
    if(count == 40){
        count = 0;
        seg++;
    }
    
    
    int num_asteroid = 0;
    for(int i = 0; i<5; i++){
       if(asteroids[i][0] != -1){
            num_asteroid++;
       } 
    }
    

}
int main(void)
{
    DDRD &= ~((1 << PD0) | (1 << PD1) | (1 << PD2) | (1 << PD3)); // set PD0, PD1, PD2, and PD3  inputs
    PORTD |= (1 << PD0) | (1 << PD1) | (1 << PD2) | (1 << PD3);   // ativa pull-ups for PD0, PD1, PD2, e PD3

    // ativa PCINT2_vect interruptpara todos os pinos PORTD
    PCICR |= (1 << PCIE2);
    PCMSK2 |= 0xFF;

    // timer
    TCCR1B |= (1 << WGM12);
    OCR1A = (TIMER_CLK / 40);
    TIMSK1 |= (1 << OCIE1A);
    TCCR1B |= (1 << CS10) | (1 << CS12); // Seta prescaler pra 1024

    sei();

    nokia_lcd_init();
    nokia_lcd_clear();
    while (1)
    {    
        nokia_lcd_clear();
        nokia_lcd_custom(1, asteroid_draw);
        nokia_lcd_custom(2, ship);
        nokia_lcd_set_cursor(ship_pos[0], ship_pos[1]);
        nokia_lcd_write_char(2, 2);
        nokia_lcd_render();

    }

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
