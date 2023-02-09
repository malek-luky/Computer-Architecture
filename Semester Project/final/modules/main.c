/* File:     main.c
 * Author:   Lukas Malek, Katerina Hobzova
 * Date:     9.6.2020
 * Course:   APO 2020
 */

///////////////////////////////////////////////////////////////////////////////
//  DECLARATIONS
///////////////////////////////////////////////////////////////////////////////

#include "thread_api.h"
struct shared_data data;
struct display_setting display;

///////////////////////////////////////////////////////////////////////////////
//  FUNCTIONS
///////////////////////////////////////////////////////////////////////////////

/* TURN OFF EVERYTHING WHAT WAS USED IN A PROGRAM */
void default_state(void)
{
  *(volatile uint32_t *)(data.led_base + SPILED_REG_LED_LINE_o) = 0;
  *(volatile uint32_t *)(data.led_base + SPILED_REG_LED_RGB1_o) = 0;
  *(volatile uint32_t *)(data.led_base + SPILED_REG_LED_RGB2_o) = 0;
  *(volatile uint32_t *)(data.motor_base + DCSPDRV_REG_DUTY_o) = 0;
  *(volatile uint32_t *)(data.motor_base + DCSPDRV_REG_PERIOD_o) = 0;
  *(volatile uint32_t *)(data.motor_base + DCSPDRV_REG_CR_o) = 0;
  *(volatile uint32_t *)(data.audio_base + AUDIOPWM_REG_PWMPER_o) = 0;
  *(volatile uint32_t *)(data.audio_base + AUDIOPWM_REG_PWM_o) = 0;
}

/* SWITCH THE TERMINAL FROM COOKED MODE INTO RAW MODE AN VICE VERSA */
void call_termios(int reset)
{
  static struct termios tio, tioOld;
  tcgetattr(STDIN_FILENO, &tio);
  if (reset)
  {
    tcsetattr(STDIN_FILENO, TCSANOW, &tioOld);
  }
  else
  {
    tioOld = tio; //backup
    cfmakeraw(&tio);
    tio.c_oflag |= OPOST;
    tcsetattr(STDIN_FILENO, TCSANOW, &tio);
  }
}

///////////////////////////////////////////////////////////////////////////////
//  MAIN PROGRAM
///////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
  /*********************** CREATES AND JOIN THREADS **************************/

  data.power = 0;
  data.quit = true;
  data.led_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
  data.audio_base = map_phys_address(AUDIOPWM_REG_BASE_PHYS, AUDIOPWM_REG_SIZE, 0);
  data.motor_base = map_phys_address(DCSPDRV_REG_BASE_PHYS_0, DCSPDRV_REG_SIZE, 0);
  data.level = 1;

  /*********************** INITIALIZATION CHECK ******************************/
  if (data.led_base == NULL ||
      data.audio_base == NULL ||
      data.motor_base == NULL)
    exit(1);

  /************************ NON-BLOCKING STDIN *******************************/
  fcntl(fileno(stdin), F_SETFL, fcntl(fileno(stdin), F_GETFL) | O_NONBLOCK);
  call_termios(0); // raw mode

  // CHECKNI JESTLI TO FUNGUJE KDYZ JE TO ZAKOMENTOVANE, POKUD JO, SMAZ TO :)
  // /////////////////////////////////////////////////////////////////////////////
  // //  INITIALIZE MUTEX
  // /////////////////////////////////////////////////////////////////////////////

  // if (pthread_mutex_init(&data.mutex, NULL))
  // {
  //   fprintf(stderr, "ERROR: Could not initialize mutex.\n");
  //   exit(100);
  // }
  // if (pthread_cond_init(&data.condvar, NULL))
  // {
  //   fprintf(stderr, "ERROR: Could not initialize condvar.\n");
  //   exit(100);
  // }

  /////////////////////////////////////////////////////////////////////////////
  //  CREATE THREADS
  /////////////////////////////////////////////////////////////////////////////
  enum
  {
    KEYBOARD_THREAD,
    KNOBS_THREAD,
    MOTOR_THREAD,
    DISPLAY_THREAD,
    GAME_THREAD,
    NUM_THREAD //returns number of threads we have
  };
  const char *thread_names[] = {
      "Keyboard",
      "Knobs",
      "Motor",
      "Game",
      "Display"};
  void *(*thd_functions[])(void *) = {
      keyboard_thread,
      knobs_thread,
      motor_thread,
      display_thread,
      game_thread};
  pthread_t threads[NUM_THREAD]; // thread container

  /**************************** CREATE THREAD ********************************/
  for (int i = 0; i < NUM_THREAD; i++)
  {
    int check = pthread_create(&threads[i], NULL, thd_functions[i], &data);
    fprintf(stderr, "\033[1;34mINFO:\033[0m %s Thread start: %s\n",
            thread_names[i],
            check ? "FAIL" : "\033[1;92mOK\033[0m");
  }

  /***************************** JOIN THREADS ********************************/
  for (int i = 0; i < NUM_THREAD; i++)
  {
    int check = pthread_join(threads[i], NULL);
    fprintf(stderr, "\033[1;34mINFO:\033[0m %s Thread joined: %s\n",
            thread_names[i],
            check ? "FAIL" : "\033[1;92mOK\033[0m");
  }

  /***************************** SET TO DEFAULT ******************************/
  default_state();
  call_termios(1);
  return 0;
}