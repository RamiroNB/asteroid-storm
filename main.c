#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "nokia5110.h"

#define TIMER_CLK (F_CPU / 1024)
#define MAX_TOP 0
#define MAX_BOTTON 74
#define MAX_RIGHT 0
#define MAX_LEFT 34

uint8_t ship[] = {0b00010000, 0b00010000, 0b00111000, 0b01111100, 0b01010100, 0b00111000, 0b00010000, 0b00010000};
uint8_t asteroid_draw[] = {0b00011000, 0b00101100, 0b00111100, 0b00110100, 0b00011000};

uint8_t asteroids[][2] = {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}};
uint8_t ship_pos[2] = {70, 15};

int count = 0;
int count_vel = 0;
int min_vel = 10;
int seg = 0;
int counter_change = 80;

int game_over = 0;
int asteroid_exists[] = {0, 0, 0, 0};
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
        if ((ship_pos[0] + 5) <= 70)
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
// timer de um seg
ISR(TIMER1_COMPA_vect)
{
    count++;
    count_vel++;
    if (count == counter_change)
    {
        count = 0;
        if (counter_change == 80)
            seg = seg + 2;
        else
            seg++;

        for (int i = 0; i < 4; i++)
        {
            if (asteroid_exists[i] != 1)
            {
                asteroid_exists[i] = 1;
                asteroids[i][0] = 0;
                asteroids[i][1] = rand() % MAX_LEFT;
                break;
            }
        }
        if (seg % 10 == 0)
        {
            if (min_vel > 0)
                min_vel = min_vel - 2;

            if (min_vel == 4)
                counter_change = 40;
        }
    }

    // condição de velocidade para movimentação
    if (count_vel >= min_vel)
    {
        count_vel = 0;
        // movinetação
        for (int i = 0; i < 4; i++)
        {
            if (asteroids[i][0] + 4 >= MAX_BOTTON || asteroids[i][0] <= -1)
            {
                asteroids[i][0] = -1;
                asteroids[i][0] = -1;
                asteroid_exists[i] = 0;
            }
            else
            {
                asteroids[i][0] = asteroids[i][0] + 4;
            }
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
        if (game_over == 0)
        {
            nokia_lcd_clear();
            nokia_lcd_custom(1, asteroid_draw);
            nokia_lcd_custom(2, ship);
            nokia_lcd_set_cursor(ship_pos[0], ship_pos[1]);
            nokia_lcd_write_char(2, 2);
            for (int i = 0; i < 4; i++)
            {
                if ((asteroids[i][0] > -1) || (asteroids[i][1] > -1)) // nao ta funcionado
                {
                    if (asteroid_exists[i] == 1) // essacondicao nao funciona
                    {
                        nokia_lcd_set_cursor(asteroids[i][0], asteroids[i][1]);
                        nokia_lcd_write_char(1, 2);
                    }
                }
                nokia_lcd_render();
            }
            // testar colisao
            // ajust collison
            for (int i = 0; i < 4; i++)
            {
                if (asteroid_exists[i] == 1)
                {
                    if ((asteroids[i][0] >= ship_pos[0] && asteroids[i][0] <= ship_pos[0] + 10) && (asteroids[i][1] >= ship_pos[1] && asteroids[i][1] <= ship_pos[1] + 10))
                    {
                        game_over = 1;
                        break;
                    }
                }
            }
        }
        else
        {

            // ajjust game over screen
            char seg_str[255];
            cli();
            nokia_lcd_clear();
            nokia_lcd_set_cursor(0, 0);
            nokia_lcd_write_string("GAME OVER", 1); // fazer glif
            nokia_lcd_set_cursor(0, 10);
            nokia_lcd_write_string("SCORE: ", 1); // fazer glif
            dtostrf(seg, 4, 2, seg_str);
            nokia_lcd_write_string(seg_str, 1); // fazer função que pega inteiro e transforma em glif virado
            nokia_lcd_render();
        }
    }
}
