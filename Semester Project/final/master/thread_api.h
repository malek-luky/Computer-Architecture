///////////////////////////////////////////////////////////////////////////////
//  DECLARATIONS
///////////////////////////////////////////////////////////////////////////////

#ifndef THREAD_API_H
#define THREAD_API_H

#define _POSIX_C_SOURCE 200112L
#define _BSD_SOURCE
#define _DEFAULT_SOURCE

#include <stdlib.h>
#include <termios.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include "thread_api.h"
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "font_types.h"

/* FUNCTIONS DEFINITION */
#define KNOB_RANGE 16777215. // used in function below
#define PERCENTAGE(power) 100 * power / KNOB_RANGE
#define MAX_POWER 16777215. // range of knobs (0x00ffffff)
#define PWM(power) (uint32_t)(100 * power / MAX_POWER)
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

/* DISPLAY COLORS */
#define LCD_COLOR_BLACK 0b0000000000000000
#define LCD_COLOR_WHITE 0b1111111111111111
#define LCD_COLOR_WHITE_BLUE 0b1110111111111111
#define LCD_COLOR_RED 0b0111100000000000
#define LCD_COLOR_GREEN 0b0000001111100000
#define LCD_COLOR_BLUE 0b0000000000001111

void *knobs_thread(void *arg);
void *keyboard_thread(void *arg);
void *motor_thread(void *arg);
void *display_thread(void *arg);
void *game_thread(void *arg);

///////////////////////////////////////////////////////////////////////////////
//  GLOBAL VARIABLES
///////////////////////////////////////////////////////////////////////////////

/* SHARED DATA */
struct shared_data
{
    int LCD_state;
    int menu_row; // 1 - 4
    float power;            // motor power, values 0-16777215
    bool quit;              // if false the program ends
    uint8_t *led_base;      // led registr base address
    uint8_t *audio_base;    // audio registr base address
    uint8_t *motor_base;    // motor registr base address
    int level;              // actual level, values 1-5
    int game_state;         // 0 failed, 1 win
    char input;             // ascii value of key which was pressed
    float goal;             // value for actual level to pass it
    double time_remaing;    // time remaining for actual level
};
extern struct shared_data data;

/* SHARED DISPLAY SETTINGS */
struct display_setting
{
    font_descriptor_t chosen_font;
    int font_size;
    uint16_t chosen_text_color;
    unsigned char *mem_base;
    unsigned char *parlcd_mem_base;
    uint16_t *LCD_buffer;
    uint16_t graph_color;
    char g_c;
    int xaxis;
};
extern struct display_setting display;

#endif