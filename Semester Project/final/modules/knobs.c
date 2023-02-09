///////////////////////////////////////////////////////////////////////////////
//  DECLARATIONS
///////////////////////////////////////////////////////////////////////////////

#include "thread_api.h"
struct shared_data data;
struct display_setting display;

///////////////////////////////////////////////////////////////////////////////
//  KNOBS THREAD
///////////////////////////////////////////////////////////////////////////////

/* REWRITE THE MODULE ACCORDING TO USER INPUT UNTIL QUIT IS SET TO ONE */
void *knobs_thread(void *arg)
{
    int change;
    uint32_t rgb_knobs_value;
    uint32_t last_value = *(volatile uint32_t *)(data.led_base +
                                                 SPILED_REG_KNOBS_8BIT_o);
    while (data.quit)
    {
        struct timespec loop_delay =
            {.tv_sec = 0, .tv_nsec = 200 * 1000 * 1000}; // sleep period 200ms
        int led_line = 0x80000000;                       // turn on the left LED
        rgb_knobs_value = *(volatile uint32_t *)(data.led_base +
                                                 SPILED_REG_KNOBS_8BIT_o);
        /************************ HANDLE BUTTON PRESS ****************************/
        if ((rgb_knobs_value & 0x0f000000) > 0) //button pressed
        {
            data.quit = false;
            printf("\n\n");
            break;
        }

        /************************** HANDLE OVERFLOW ******************************/
        change = abs(rgb_knobs_value - last_value);
        if ((change & 0xff0000) < 0xc00000 &&
            (change & 0xff00) < 0xc000 &&
            (change & 0xff) < 0xc0)
            change = 2 * (rgb_knobs_value - last_value); // calculates the position
        else
            change = 0;
        last_value = rgb_knobs_value;

        /*************************** ADJUST POWER ********************************/
        data.power += change;
        data.power = MAX(MIN(data.power, MAX_POWER), -MAX_POWER);
        for (int i = 0; i < abs(32 * (data.power / MAX_POWER)); i++)
        {
            led_line = led_line >> 1;
        }
        *(volatile uint32_t *)(data.led_base + SPILED_REG_LED_LINE_o) = led_line;

        printf("\033[1;34mPOWER:\033[0m     %3.3f%c\n", PERCENTAGE(data.power), '%');
        printf("\033[1;34mTIME LEFT:\033[0m %-2.0fs\r", data.time_remaing);
        printf("\033[1A");
        clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
    }
    return NULL;
}