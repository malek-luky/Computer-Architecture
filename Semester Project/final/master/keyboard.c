///////////////////////////////////////////////////////////////////////////////
//  DECLARATIONS
///////////////////////////////////////////////////////////////////////////////

#include "thread_api.h"

struct shared_data data;
struct display_setting display;
int x_axis2[] = {0, 10, 30, 60, 120};

///////////////////////////////////////////////////////////////////////////////
//  FUNCTIONS
///////////////////////////////////////////////////////////////////////////////

/* SET GRAPH COLOR FROM MENU */
uint16_t set_graph_color(void)
{
    uint16_t ret = LCD_COLOR_BLACK;
    switch (data.menu_row)
    {
    case 1:
        ret = LCD_COLOR_WHITE;
        display.g_c = 'w';
        break;
    case 2:
        ret = LCD_COLOR_BLUE;
        break;
    case 3:
        ret = LCD_COLOR_GREEN;
        break;
    case 4:
        ret = LCD_COLOR_RED;
        break;
    default:
        break;
    }
    return ret;
}

/* MOVE USER FROM MENU TO GRAPH AFTER SELECTION */
void confirm_selection(void)
{
    if (data.LCD_state == 1)
    {

        {
            if (display.graph_color == 0)
            {
                data.LCD_state = 2;
            }
            display.graph_color = set_graph_color();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
//  KEYBOARD THREAD
///////////////////////////////////////////////////////////////////////////////

/* RECEIVE THE USER INPUT FROM COMMAND PROMPT */
void *keyboard_thread(void *arg)
{
    while (data.quit)
    {
        data.input = fgetc(stdin);
        if (data.input != 255)
        {
            switch (data.input)
            {
            case 'm':
                data.LCD_state = 1;
                break;
            case 13:
                confirm_selection();
                break;
            case 65: //up
                data.menu_row--;
                data.menu_row = abs(data.menu_row % 5);
                if (data.menu_row == 0)
                {
                    data.menu_row = 4;
                }
                break;
            case 66: //down
                data.menu_row++;
                data.menu_row = abs(data.menu_row % 5);
                if (data.menu_row == 0)
                {
                    data.menu_row = 1;
                }
                break;

            case '+':
                data.power += 0x010000;
                break;
            case '-':
                data.power -= 0x010000;
                break;
            case 'q':
                data.LCD_state = 3; //goodbye screen
                break;
            }
        }
    }
    return NULL;
}