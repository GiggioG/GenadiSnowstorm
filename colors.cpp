#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <winuser.h>
#include <time.h>
#include <cmath>
#include <vector>
using namespace std;

const int MAP_ROWS=80;
const int MAP_COLS=100;


HANDLE hConsoleOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
COORD screen_buf = {MAP_ROWS , MAP_COLS};
CHAR_INFO blank_screen[MAP_ROWS * MAP_COLS] = {0};

enum COLORS {
    BLACK = 0,
    BLUE = FOREGROUND_BLUE,
    CYAN = FOREGROUND_BLUE | FOREGROUND_GREEN,
    GREEN = FOREGROUND_GREEN,
    RED = FOREGROUND_RED,
    BROWN = FOREGROUND_RED | FOREGROUND_GREEN,
    PURPLE = FOREGROUND_RED | FOREGROUND_BLUE,
    LIGHT_GREY =  FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN,

    GREY = 0 | FOREGROUND_INTENSITY,
    LIGHT_BLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    LIGHT_CYAN = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    LIGHT_GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    LIGHT_RED = FOREGROUND_RED | FOREGROUND_INTENSITY,
    YELLOW = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    PINK = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    WHITE =  FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY
};
COLORS BKG_COL = GREY;


void draw_char(char ch, int y, int x, COLORS foreground_color, COLORS background_color) {
    CHAR_INFO ch_info;
    ch_info.Char.AsciiChar = ch;
    ch_info.Attributes = foreground_color | (background_color << 4);

    COORD buf_size = {1, 1};
    COORD buf_coord = {0, 0};
    SMALL_RECT screen_pos = {x, y, x+1, y+1};
    ::WriteConsoleOutput(hConsoleOutput, &ch_info, buf_size, buf_coord, &screen_pos);

}

void clear_screen() {
    COORD buf_coord = {0, 0};
    SMALL_RECT screen_pos = {0, 0, screen_buf.X, screen_buf.Y};
    ::WriteConsoleOutput(hConsoleOutput, blank_screen, screen_buf, buf_coord, &screen_pos);
}



void set_background() {
    for (int i = 0; i < screen_buf.X; i++) {
        for (int j = 0; j < screen_buf.Y; j++) {
             draw_char(' ', i, j, WHITE, BKG_COL);
        }
    }
}
