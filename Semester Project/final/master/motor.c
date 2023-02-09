///////////////////////////////////////////////////////////////////////////////
//  DECLARATIONS
///////////////////////////////////////////////////////////////////////////////

#include "thread_api.h"
struct shared_data data;
struct display_setting display;

///////////////////////////////////////////////////////////////////////////////
//  DC MOTOR THREAD
///////////////////////////////////////////////////////////////////////////////

/* CHANGE THE POWER */
void *motor_thread(void *arg)
{
    /* Change motor speed - frequency is 20 kHz */
    *(volatile uint32_t *)(data.motor_base + DCSPDRV_REG_CR_o) = 0;
    *(volatile uint32_t *)(data.motor_base + DCSPDRV_REG_DUTY_o) = 0;
    *(volatile uint32_t *)(data.motor_base + DCSPDRV_REG_PERIOD_o) = 0;
    *(volatile uint32_t *)(data.motor_base + DCSPDRV_REG_PERIOD_o) = 5000;
    *(volatile uint32_t *)(data.motor_base + DCSPDRV_REG_CR_o) = DCSPDRV_REG_CR_PWM_ENABLE_m;

    struct timespec loop_delay =
        {.tv_sec = 0, .tv_nsec = 50 * 1000 * 1000}; // sleep period 10ms

    while (data.quit)
    {
        int pwm = PERCENTAGE(data.power) * 50;
        if (pwm > 0)
        {
            *(volatile uint32_t *)(data.motor_base + DCSPDRV_REG_DUTY_o) =
                pwm | DCSPDRV_REG_DUTY_DIR_A_m;
        }
        else if (pwm == 0)
        {
            *(volatile uint32_t *)(data.motor_base + DCSPDRV_REG_DUTY_o) = 0;
        }
        else if (pwm < 0)
        {
            pwm = -pwm;
            *(volatile uint32_t *)(data.motor_base + DCSPDRV_REG_DUTY_o) =
                pwm | DCSPDRV_REG_DUTY_DIR_B_m;
        }
        clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
    }
    return NULL;
}