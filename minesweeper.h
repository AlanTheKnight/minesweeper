#include <vector>

#ifndef MINESWEEPER_H
#define MINESWEEPER_H

struct cell
{
    bool flag = false;
    bool opened = false;
    int value = 0;
};


#define vect2d std::vector<std::vector<cell>>

class Minesweeper
{
private:
    void generate_cell(int x, int y);
    bool check(int x, int y);
    void spawn_bombs(int amount);
    void generate_field();
    void open_empty_cell(int x, int y);
public:
    Minesweeper(int h, int w, int a = 0);
    void print_field();
    vect2d field;
    bool check_for_win();
    bool open_cell(int x, int y);
    int height, width;
    void changeFlagState(int x, int y);
};

#endif