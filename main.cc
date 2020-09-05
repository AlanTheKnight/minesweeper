#include "minesweeper.h"
#include <ncurses.h>
#include <SFML/Audio.hpp>
#include <thread>
#include <chrono>


struct position
{
    int x;
    int y;
};

void printField(Minesweeper m, position selected)
{
    for (int y = 0; y < m.height; y++) {
        for (int x = 0; x < m.width; x++) {
            cell current = m.field[y][x];
            if (selected.x == x && selected.y == y)
                attron(A_UNDERLINE);
            if (current.opened && current.value != 0 && !current.flag) {
                attron(COLOR_PAIR(current.value));
                printw("%d ", current.value);
                attroff(COLOR_PAIR(current.value));
            } else if (current.flag) {
                printw("P ");
            } else if (current.value == 0 && current.opened) {
                attron(COLOR_PAIR(current.value));
                printw("  ", current.value);
                attroff(COLOR_PAIR(current.value));
            } else {
                attron(COLOR_PAIR(11));
                printw("  ");
                attroff(COLOR_PAIR(11));
            }
            if (selected.x == x && selected.y == y)
                attroff(A_UNDERLINE);
        }
        printw("\n");
    }
}


void playWav(std::string filename)
{
    sf::Music music;
    bool status = music.openFromFile(filename);
    music.play();
    int seconds = music.getDuration().asSeconds();
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

int main(int argc, char const *argv[])
{
    initscr();
    keypad(stdscr, true);
    use_default_colors();
    start_color();

    if (argc < 3)
    {
        printw("Not enough arguments.");
        getch();
        endwin();
        return 0;
    }
    int w = atoi(argv[1]), h = atoi(argv[2]);

    init_pair(0, COLOR_BLUE, -1);
    init_pair(1, COLOR_GREEN, -1);
    init_pair(2, COLOR_RED, -1);
    init_pair(3, COLOR_RED, -1);
    init_pair(4, COLOR_BLUE, -1);
    init_pair(5, COLOR_BLUE, -1);
    init_pair(6, COLOR_BLUE, -1);
    init_pair(7, COLOR_GREEN, -1);
    init_pair(8, COLOR_BLUE, -1);
    init_pair(9, COLOR_WHITE, -1);
    init_pair(11, -1, COLOR_YELLOW);

    init_pair(10, 0, COLOR_WHITE);

    Minesweeper m(20, 20, 30);
    position sel{0, 0};

    int exit_status = 0;

    while (true)
    {
        clear();
        printField(m, sel);
        switch (getch())
        {
        case KEY_UP:
            if (sel.y > 0)
                sel.y--;
            break;

        case KEY_DOWN:
            if (sel.y < m.height - 1)
                sel.y++;
            break;

        case KEY_LEFT:
            if (sel.x > 0)
                sel.x--;
            break;
        
        case KEY_RIGHT:
            if (sel.x < m.width - 1)
                sel.x++;
            break;

        case ' ':
            if (!m.open_cell(sel.x, sel.y))
                exit_status = 1;
            if (m.check_for_win())
                exit_status = 2;
            break;

        case '\n':
            m.changeFlagState(sel.x, sel.y);
            if (m.check_for_win())
                exit_status = 2;
            break;
        
        default:
            break;
        }

        if (exit_status)
            break;
    }

    if (exit_status == 1) {
        clear();
        attron(COLOR_PAIR(9));
        mvprintw(getmaxy(stdscr) / 2, (getmaxx(stdscr) - 1) / 2, "You lost!");
        attroff(COLOR_PAIR(9));
        refresh();
        playWav("sounds/explosition.wav");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    } else if (exit_status == 2) {
        clear();
        attron(COLOR_PAIR(9));
        mvprintw(getmaxy(stdscr) / 2, (getmaxx(stdscr) - 1) / 2, "You win!");
        attroff(COLOR_PAIR(9));
        refresh();
        playWav("sounds/win.wav");
    }


    endwin();
    return 0;
}
