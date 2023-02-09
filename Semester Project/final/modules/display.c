
///////////////////////////////////////////////////////////////////////////////
//  DECLARATIONS
///////////////////////////////////////////////////////////////////////////////

#include "thread_api.h"

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320
#define BUFFER_SIZE (SCREEN_SIZE * 2)
#define NUMBER_OF_ROWS 5
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT)

struct shared_data data;
struct display_setting display;

char *colors[] = {" Graph color ", " White ", " Blue ", " Green ", " Red "};
char *x_axis[] = {" Range of X-axis ",
                  " 10 ",
                  " 30 ",
                  " 60 ",
                  " 120 "};

/* DECLARATION OF FUNCTIONS */
void draw_line(int x0, int y0, int x1, int y1, uint16_t color);
void string_to_buffer(int x, int y, char *string, uint16_t color);
void print_buffer();
void draw_triangle_side(int x0, int y0, int size);
void draw_triangle_down(int x0, int y0, int size);
void draw_triangle_up(int x0, int y0, int size);
void letter_to_buffer(int x, int y, char letter, uint16_t color);
void draw_axes(int x0, int y0, int x1, int y1);
void draw_frame(int x0, int y0, int x1, int y1);
void reset_buffer(void);
void redraw_graph(void);
void draw_box(int x0, int y0, int x1, int y1, int lines, char **names);
int center(char *word, int middle);
void congratulation(void);
void game_over(void);
void goodbye_screen(void);
void graph(void);
void welcome(void);
int lenght_counter(char *word);
void draw_menu(void);
void menu(void);
void font_size(int size);
void current_box(void);
void my_assert(bool r, const char *fcname, int line, const char *fname);
void select_font(font_descriptor_t font);
void display_color(uint16_t color);
void display_init(void);

///////////////////////////////////////////////////////////////////////////////
//  LCD DISPLAY THREAD
///////////////////////////////////////////////////////////////////////////////

/* CONTROL THE LCD */
void *display_thread(void *arg)
{
    display_init();
    while (data.quit)
    {
        switch (data.LCD_state)
        {
        case 0:
            welcome();
            break;
        case 1:
            menu(); //go to menu
            break;
        case 2:
            graph();
            break;
        case 3:
            data.quit = false;
            break;
        default:
            break;
        }
    }
    if (data.game_state == 1)
    {
        congratulation();
        sleep(1);
    }
    else
    {
        game_over();
        sleep(1);
    }
    goodbye_screen();

    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  FUNCTIONS
///////////////////////////////////////////////////////////////////////////////

/* INITIALIZE DISPLAY AND NEEDED VARIABLES*/
void display_init(void)
{
    display.mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    my_assert(display.mem_base != NULL, __func__, __LINE__, __FILE__);
    display.parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS,
                                               PARLCD_REG_SIZE, 0);
    parlcd_hx8357_init(display.parlcd_mem_base);
    my_assert(display.parlcd_mem_base != NULL, __func__, __LINE__, __FILE__);
    font_size(2);
    data.LCD_state = 0;
    select_font(font_rom8x16); //mby change
    display_color(LCD_COLOR_WHITE);
    display.LCD_buffer = (uint16_t *)malloc(BUFFER_SIZE); //mby my_malloc
    reset_buffer();
    data.all_selected = 0;
    display.graph_color = 0;
    display.xaxis = 0;
    display.g_c = 'a';
    data.menu_row = 1;
    data.menu_box = 0;
}

/* SELECTION OF DEFAULT DISPLAY COLOR */
void display_color(uint16_t color)
{
    display.chosen_text_color = color;
}

/* SELECTION OF FONT */
void select_font(font_descriptor_t font)
{
    display.chosen_font = font;
}

/* MODIFIED ASSERT */
void my_assert(bool r, const char *fcname, int line, const char *fname)
{
    if (!r)
    {
        fprintf(stderr, "ERROR : My_assert failed: %s() line %d in %s!\n",
                fcname, line, fname);
        exit(1);
    }
}

/* HIGHLIGHT CURRENTLY SELECTED FIELD IN MENU BOX */
void current_box(void)
{
    int x0 = 0;
    int x1 = 0;
    int y0 = 60;
    int y1 = SCREEN_HEIGHT - 10;
    int tmp_h = (y1 - y0) / 5;
    y0 += data.menu_row * tmp_h;
    y1 = y0 + tmp_h;
    if (data.menu_box == 0)
    {
        x0 = 10;
        x1 = SCREEN_WIDTH / 2 - 5;
        //x1 = SCREEN_WIDTH - 10;
    }
    else if (data.menu_box == 1)
    {
        x0 = SCREEN_WIDTH / 2 + 5;
        x1 = SCREEN_WIDTH - 10;
    }
    for (int i = y0 + 1; i < y1; ++i)
    {
        for (int j = x0 + 1; j < x1; ++j)
        {
            display.LCD_buffer[i * SCREEN_WIDTH + j] = LCD_COLOR_WHITE;
        }
    }
    if (data.menu_box == 0)
    {
        string_to_buffer(x0 + center(colors[data.menu_row], (x1 - x0) / 2),
                         y0 + (y1 - y0) / 2, colors[data.menu_row], LCD_COLOR_WHITE);
    }
    else if (data.menu_box == 1)
    {
        string_to_buffer(x0 + center(x_axis[data.menu_row], (x1 - x0) / 2),
                         y0 + (y1 - y0) / 2, x_axis[data.menu_row], LCD_COLOR_WHITE);
    }
    print_buffer();
}

/* CHANGE FONT SIZE */
void font_size(int size)
{
    display.font_size = size;
}

/* MENU SCREEN */
void menu(void)
{
    while (data.LCD_state == 1)
    {
        draw_menu();
        current_box();
    }
}

/* MENU GUI */
void draw_menu(void)
{
    reset_buffer();
    font_size(2);
    string_to_buffer(center("MENU", SCREEN_WIDTH / 2) - 2, 20, "MENU", display.chosen_text_color);
    font_size(1);
    draw_box(10, 60, SCREEN_WIDTH / 2 - 5, SCREEN_HEIGHT - 10, NUMBER_OF_ROWS - 1, colors);
    draw_box(SCREEN_WIDTH / 2 + 5, 60, SCREEN_WIDTH - 10, SCREEN_HEIGHT - 10, NUMBER_OF_ROWS - 1, x_axis); //might be finished later
}

/* LENGHT OF STRING */
int lenght_counter(char *word)
{
    my_assert(word != NULL, __func__, __LINE__, __FILE__);
    int i = 0;
    while (word[i] != '\n' && word[i])
    {
        ++i;
    }
    return i;
}

/* WELCOME SCREEN */
void welcome(void)
{
    font_size(2);
    draw_frame(10, 10, SCREEN_WIDTH - 10, SCREEN_HEIGHT - 10);
    string_to_buffer(center("WELCOME", SCREEN_WIDTH / 2), 100,
                     "WELCOME", display.chosen_text_color);
    string_to_buffer(center("SEMESTARL WORK", SCREEN_WIDTH / 2), 150,
                     "SEMESTARL WORK", display.chosen_text_color);
    string_to_buffer(center("MALEK & HOBZOVA", SCREEN_WIDTH / 2), 200,
                     "MALEK & HOBZOVA", display.chosen_text_color);
    print_buffer();
}

/* GRAPH SCREEN */
void graph(void)
{
    reset_buffer();
    font_size(2);
    string_to_buffer(10, 15, "f", display.chosen_text_color);
    string_to_buffer(SCREEN_WIDTH - 30, 40 + (SCREEN_HEIGHT - 40) / 2, "t", display.chosen_text_color);
    draw_axes(40, 40, SCREEN_WIDTH - 59, SCREEN_HEIGHT - 40);
    print_buffer();
    while (data.LCD_state == 2 && data.quit)
    {
        sleep(0.2);
        redraw_graph();
        draw_axes(40, 40, SCREEN_WIDTH - 59, SCREEN_HEIGHT - 40);
        draw_triangle_up(40, 32, 8);
        draw_triangle_down(40, SCREEN_HEIGHT - 40, 8);
        draw_triangle_side(SCREEN_WIDTH - 59, 40 + (SCREEN_HEIGHT - 80) / 2, 8);
        print_buffer();
    }
}

/* PRINT GOOD-BYE SCREEN */
void goodbye_screen(void)
{
    reset_buffer();
    font_size(5);
    draw_frame(10, 10, SCREEN_WIDTH - 10, SCREEN_HEIGHT - 10);
    string_to_buffer(center("GOOD BYE", SCREEN_WIDTH / 2),
                     120, "GOOD BYE", display.chosen_text_color);
    print_buffer();
}

/* PRINT GAME-OVER SCREEN */
void game_over(void)
{
    reset_buffer();
    font_size(5);
    draw_frame(10, 10, SCREEN_WIDTH - 10, SCREEN_HEIGHT - 10);
    string_to_buffer(center("GAME-OVER", SCREEN_WIDTH / 2), 120,
                     "GAME-OVER", display.chosen_text_color);
    print_buffer();
}

/* PRINT CONGRATULATION SCREEN */
void congratulation(void)
{
    reset_buffer();
    font_size(4);
    draw_frame(10, 10, SCREEN_WIDTH - 10, SCREEN_HEIGHT - 10);
    string_to_buffer(center("CONGRATULATION", SCREEN_WIDTH / 2), 120,
                     "CONGRATULATION", display.chosen_text_color);
    print_buffer();
}

/* FIND START POSITION FOR STRING TO BE CENTERIC */
int center(char *word, int middle)
{
    int start;
    int len = lenght_counter(word);
    start = middle - (len / 2 * display.chosen_font.maxwidth * display.font_size);
    return start;
}

/* DRAW BOX ON LCD */
void draw_box(int x0, int y0, int x1, int y1, int lines, char **names)
{
    draw_frame(x0, y0, x1, y1);
    int tmp_h = (y1 - y0) / (lines + 1);
    for (int i = 0; i <= lines; ++i)
    {
        draw_line(x0, y0 + i * tmp_h, x1, y0 + i * tmp_h, display.chosen_text_color);
        string_to_buffer(x0 + center(names[i], (x1 - x0) / 2),
                         y0 + (i + 1) * tmp_h - tmp_h / 2, names[i], display.chosen_text_color);
    }
}

/* REDRAW VALUES ON GRAPH */
void redraw_graph(void)
{
    int my_power = PERCENTAGE(data.power);
    my_power *= (20000 / 100); //20kH max frequency, 100 percent
    my_power /= 240;           // height of Y-axis

    for (int i = 40; i <= (SCREEN_HEIGHT - 40); ++i)
    {
        for (int j = 41; j <= (SCREEN_WIDTH - 59); ++j)
        {

            if (j == 41 && display.LCD_buffer[i * SCREEN_WIDTH + j] == display.graph_color)
            {
                display.LCD_buffer[i * SCREEN_WIDTH + j] = LCD_COLOR_BLACK;
            }
            else if (display.LCD_buffer[i * SCREEN_WIDTH + j] ==
                         display.graph_color &&
                     j > 41)
            {
                display.LCD_buffer[i * SCREEN_WIDTH + j - 1] = display.graph_color;
                display.LCD_buffer[i * SCREEN_WIDTH + j] = LCD_COLOR_BLACK;
            }
            else if (j >= (SCREEN_WIDTH - 59) && i == 160 - my_power)
            {
                display.LCD_buffer[i * SCREEN_WIDTH + j] = display.graph_color;
                display.LCD_buffer[(i + 1) * SCREEN_WIDTH + j] = display.graph_color;
                display.LCD_buffer[(i - 1) * SCREEN_WIDTH + j] = display.graph_color;
            }
            else if (display.LCD_buffer[i * SCREEN_WIDTH + j] == display.chosen_text_color && j > 41)
            {
                display.LCD_buffer[i * SCREEN_WIDTH + j] = LCD_COLOR_BLACK;
            }
        }
    }
}

/* FILL BUFFER WITH DEFAULT COLOR */
void reset_buffer(void)
{
    for (int i = 0; i < SCREEN_SIZE; ++i)
    {
        display.LCD_buffer[i] = LCD_COLOR_BLACK;
    }
}

/* DRAW BORDERLINES */
void draw_frame(int x0, int y0, int x1, int y1)
{
    draw_line(x0, y0, x1, y0, display.chosen_text_color); // UP
    draw_line(x1, y0, x1, y1, display.chosen_text_color); // RIGHT
    draw_line(x0, y1, x1, y1, display.chosen_text_color); // DOWN
    draw_line(x0, y0, x0, y1, display.chosen_text_color); // LEFT
}

/* DRAW GRAPH AXES */
void draw_axes(int x0, int y0, int x1, int y1)
{
    draw_line(x0, y0 + (y1 - y0) / 2, x1, y0 + (y1 - y0) / 2, display.chosen_text_color); // X - AXIS
    draw_line(x0, y0, x0, y1, display.chosen_text_color);                                 // Y - AXIS

    draw_line(x0 + (x1 - x0) / 2, y0 + (y1 - y0) / 2 - 10, x0 + (x1 - x0) / 2, y0 + (y1 - y0) / 2 + 10, display.chosen_text_color);
    draw_line(x0 + (x1 - x0) / 4, y0 + (y1 - y0) / 2 - 10, x0 + (x1 - x0) / 4, y0 + (y1 - y0) / 2 + 10, display.chosen_text_color);
    draw_line(x0 + 3 * (x1 - x0) / 4, y0 + (y1 - y0) / 2 - 10, x0 + 3 * (x1 - x0) / 4, y0 + (y1 - y0) / 2 + 10, display.chosen_text_color);
    draw_line(x0 - 5, y0 + (y1 - y0) / 4, x0 + 5, y0 + (y1 - y0) / 4, display.chosen_text_color);
    draw_line(x0 - 5, y0 + 3 * (y1 - y0) / 4, x0 + 5, y0 + 3 * (y1 - y0) / 4, display.chosen_text_color);
}

/* PUSH ONE LETTER TO BUFFER */
void letter_to_buffer(int x, int y, char letter, uint16_t color)
{
    const uint16_t *from = &display.chosen_font.bits[letter * display.chosen_font.height];
    for (uint16_t j = 0; j < display.chosen_font.height * display.font_size; ++j)
    {
        for (uint16_t i = 0; i < display.chosen_font.maxwidth * display.font_size; ++i)
        {
            display.LCD_buffer[x + i + (y + j) * SCREEN_WIDTH] =
                ((from[(int)(j / display.font_size)] >>
                  (16 - ((int)(i / display.font_size)))) &
                 1) *
                color;
        }
    }
}

void draw_triangle_up(int x0, int y0, int size)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < i; ++j)
        {
            draw_line(x0 - j, y0 + j, x0 + j, y0 + j, display.chosen_text_color);
        }
    }
}

void draw_triangle_down(int x0, int y0, int size)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < i; ++j)
        {
            draw_line(x0 - size + j + 2, y0 + j, x0 + size - j - 2, y0 + j, display.chosen_text_color);
        }
    }
}

void draw_triangle_side(int x0, int y0, int size)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < i; ++j)
        {
            draw_line(x0 + j, y0 - size + j + 2, x0 + j, y0 + size - j - 2, display.chosen_text_color);
        }
    }
}

/* PRINT WHOLE BUFFER TO LCD */
void print_buffer()
{
    parlcd_write_cmd(display.parlcd_mem_base, 0x2c);
    for (int i = 0; i < SCREEN_HEIGHT; ++i)
    {
        for (int j = 0; j < SCREEN_WIDTH; ++j)
        {
            parlcd_write_data(display.parlcd_mem_base, display.LCD_buffer[j + i * SCREEN_WIDTH]);
        }
    }
}

/* PUSH WORD/STRING TO BUFFER */
void string_to_buffer(int x, int y, char *string, uint16_t color)
{
    for (int i = 0; string[i]; ++i)
    {
        letter_to_buffer(x + (i * display.chosen_font.maxwidth * display.font_size), y, string[i], color);
    }
}

/* DRAW ONE SRAIGHT LINE */
void draw_line(int x0, int y0, int x1, int y1, uint16_t color)
{
    int dist_x = abs(x1 - x0);
    int step_x = x0 < x1 ? 1 : -1;
    int dist_y = -abs(y1 - y0);
    int step_y = y0 < y1 ? 1 : -1;
    int dist_sum = dist_x + dist_y;
    int double_dist_sum = 2 * dist_sum;
    while (1)
    {
        display.LCD_buffer[y0 * SCREEN_WIDTH + x0] = color;
        if (x0 == x1 && y0 == y1)
        {
            break;
        }
        if (double_dist_sum >= dist_y)
        {
            dist_sum += dist_y;
            x0 += step_x;
        }
        if (double_dist_sum <= dist_x)
        {
            dist_sum += dist_x;
            y0 += step_y;
        }
    }
}