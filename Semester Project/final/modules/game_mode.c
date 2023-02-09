///////////////////////////////////////////////////////////////////////////////
//  DECLARATIONS
///////////////////////////////////////////////////////////////////////////////

#include "thread_api.h"

#define G1 382219
#define A1 227272
#define B1 214518
#define C2 190385
#define D2 170262
#define A2 113636
#define B2 107258

struct shared_data data;
struct display_setting display;

///////////////////////////////////////////////////////////////////////////////
//  FUNCTIONS
///////////////////////////////////////////////////////////////////////////////

/* PLAY THE TONE FOR 120MS */
void tone_short(int tone)
{
    struct timespec loop_delay = {.tv_sec = 0, .tv_nsec = 120 * 1000 * 1000};
    *(volatile uint32_t *)(data.audio_base + AUDIOPWM_REG_PWM_o) = tone / 10;
    *(volatile uint32_t *)(data.audio_base + AUDIOPWM_REG_PWMPER_o) = tone;
    clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
    *(volatile uint32_t *)(data.audio_base + AUDIOPWM_REG_PWM_o) = 0;
    *(volatile uint32_t *)(data.audio_base + AUDIOPWM_REG_PWMPER_o) = 0;
    loop_delay.tv_nsec = 50 * 1000 * 1000;
    clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
}

/* PLAY THE TONE FOR 240MS */
void tone_long(int tone)
{
    struct timespec loop_delay = {.tv_sec = 0, .tv_nsec = 240 * 1000 * 1000};
    *(volatile uint32_t *)(data.audio_base + AUDIOPWM_REG_PWM_o) = tone / 10;
    *(volatile uint32_t *)(data.audio_base + AUDIOPWM_REG_PWMPER_o) = tone;
    clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
    *(volatile uint32_t *)(data.audio_base + AUDIOPWM_REG_PWM_o) = 0;
    *(volatile uint32_t *)(data.audio_base + AUDIOPWM_REG_PWMPER_o) = 0;
    loop_delay.tv_nsec = 50 * 1000 * 1000;
    clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
}

/* DISCO DISCO PARTY PARTY */
void disco_disco_party_party()
{
    //https://www.youtube.com/watch?v=vrphLUWZv3Q
    *(volatile uint32_t *)(data.led_base + SPILED_REG_LED_RGB1_o) =
        rand() % 16777215;
    *(volatile uint32_t *)(data.led_base + SPILED_REG_LED_RGB2_o) =
        rand() % 16777215;
    *(volatile uint32_t *)(data.led_base + SPILED_REG_LED_LINE_o) =
        (*(volatile uint32_t *)(data.led_base + SPILED_REG_LED_LINE_o) == 0)
            ? 0xffffffff
            : 0;
}

/* PLAY THE SONG COFFIN DANCE */
void coffin_dance(void)
{
    tone_long(G1);
    disco_disco_party_party();
    tone_short(G1);
    disco_disco_party_party();
    tone_short(D2);
    disco_disco_party_party();
    tone_long(C2);
    disco_disco_party_party();
    tone_long(B1);
    disco_disco_party_party();
    tone_long(A1);
    disco_disco_party_party();
    tone_short(A1);
    disco_disco_party_party();
    tone_short(A1);
    disco_disco_party_party();
    tone_long(C2);
    disco_disco_party_party();
    tone_short(B1);
    disco_disco_party_party();
    tone_short(A1);
    disco_disco_party_party();
    tone_long(G1);
    disco_disco_party_party();
    tone_short(G1);
    disco_disco_party_party();
    tone_short(B2);
    disco_disco_party_party();
    tone_short(A2);
    disco_disco_party_party();
    tone_short(B2);
    disco_disco_party_party();
    tone_short(A2);
    disco_disco_party_party();
    tone_short(B2);
    disco_disco_party_party();
    tone_long(G1);
    disco_disco_party_party();
    tone_short(G1);
    disco_disco_party_party();
    tone_short(B2);
    disco_disco_party_party();
    tone_short(A2);
    disco_disco_party_party();
    tone_short(B2);
    disco_disco_party_party();
    tone_short(A2);
    disco_disco_party_party();
    tone_short(B2);
}

///////////////////////////////////////////////////////////////////////////////
//  GAME THREAD
///////////////////////////////////////////////////////////////////////////////

/* CONTROL THE GAME */
void *game_thread(void *arg)
{
    /******************* GENERATE THE GOAL FOR CURRENT LEVEL *******************/
    data.game_state = 0;
    srand(time(NULL));
    data.goal = (2 * (float)rand() - RAND_MAX) / ((float)RAND_MAX / 100);
    printf("\033[1;34mLEVEL %d/5:\033[0m %3.3f%c\n", data.level, data.goal, '%');

    /*************************** TURN ON THE LED *******************************/
    *(volatile uint32_t *)(data.led_base + SPILED_REG_LED_RGB1_o) = 0x00ff00;
    *(volatile uint32_t *)(data.led_base + SPILED_REG_LED_RGB2_o) = 0x00ff00;

    /************************* START THE COUNTDOWN *****************************/

    struct timespec t_start, t_level, t_now;
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    clock_gettime(CLOCK_MONOTONIC, &t_level);
    clock_gettime(CLOCK_MONOTONIC, &t_now);
    struct timespec loop_delay = // 50ms
        {.tv_sec = 0, .tv_nsec = 50 * 1000 * 1000};
    while (data.quit)
    {
        clock_gettime(CLOCK_MONOTONIC, &t_now);
        data.time_remaing = 60 / data.level -
                            (t_now.tv_sec - t_level.tv_sec);

        /*************************** GAME OVER ***********************************/
        if (data.time_remaing < 0)
        {
            printf("\n\n\033[1;31mGAME OVER :(\033[0m\n");
            data.quit = false;
        }

        /************************* CONGRATULATIONS *******************************/
        if (PERCENTAGE(data.power) > data.goal - 0.005 &&
            PERCENTAGE(data.power) < data.goal + 0.005)
        {
            if (data.level == 5)
            {
                clock_gettime(CLOCK_MONOTONIC, &t_now);
                double time_taken = (t_start.tv_sec - t_now.tv_sec);
                printf("\n\n\033[1;33mGONGRATULATION!!! Your time: %3.3fs\033[0m\n",
                       time_taken);
                data.quit = false;
                data.game_state = 1;
                coffin_dance();
            }
            /**************************** LEVEL UP *********************************/
            else
            {
                data.level++;
                data.goal = (2 * (float)rand() - RAND_MAX) / ((float)RAND_MAX / 100);
                printf("\033[1A"); // move up 1 line
                printf("\033[1;34mLEVEL %d/5:\033[0m %3.3f%c\n",
                       data.level, data.goal, '%');
                tone_long(A2);
                clock_gettime(CLOCK_MONOTONIC, &t_level); // reset the countdown
            }
        }
        clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
    }
    return NULL;
}