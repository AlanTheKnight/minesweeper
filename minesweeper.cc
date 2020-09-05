#include "minesweeper.h"
#include <random>
#include <iostream>
#include <SFML/Audio.hpp>


Minesweeper::Minesweeper(int h, int w, int a) : field(h, std::vector<cell>(w))
{
    if (a == 0)
        a = h * w / 10;
    height = h;
    width = w;
    spawn_bombs(a);
    generate_field();
}


void Minesweeper::print_field()
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (field[i][j].opened == true)
                std::cout << field[i][j].value << ' ';
            else
                std::cout << "  ";
        }
        std::cout << '\n';
    }
}


bool Minesweeper::check(int x, int y)
{
    return (
        x >= 0 && y >= 0 &&
        x < width && y < height
    );
}


bool Minesweeper::open_cell(int x, int y)
{
    // Return false if cell has a bomb
    if (field[y][x].value == 9)
        return false;
    if (field[y][x].value == 0)
        open_empty_cell(x, y);
    field[y][x].opened = true;
    return true;
}


void Minesweeper::generate_cell(int x, int y)
{
    if (field[y][x].value == 9)
        return;
    int bomb_count = 0;
    for (int i = y - 1; i < y + 2; i++)
    {
        for (int j = x - 1; j < x + 2; j++)
        {
            if (check(j, i) && field[i][j].value == 9) {
                bomb_count++;
            }
        }
    }
    field[y][x].value = bomb_count;
}


void Minesweeper::generate_field()
{
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            generate_cell(i, j);
        }
    }
}


void Minesweeper::spawn_bombs(int amount)
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> gen_w(0, width - 1);
    std::uniform_int_distribution<int> gen_h(0, height- 1);
    for (int i = 0; i < amount; i++)
    {
        field[gen_h(rng)][gen_w(rng)] = cell{
            false, false, 9
        };
    }
}


bool Minesweeper::check_for_win()
{
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cell current = field[i][j];
            if (
                // Cell is a bomb but it isn't flagged
                (current.value == 9 && current.flag == false) ||
                // Cell isn't a bomb but it isn't opened
                (current.value != 9 && current.opened == false)
                )
                return false;
        }
    }
    return true;
}


void Minesweeper::open_empty_cell(int x, int y)
{
    if (field[y][x].opened)
        return;
    if (field[y][x].value != 0 && field[y][x].value != 9) {
        field[y][x].opened = true;
        return;
    }
    field[y][x].opened = true;
    for (int i = y - 1; i < y + 2; i++) {
        for (int j = x - 1; j < x + 2; j++) {
            if (check(j, i) && !(j == x && i == y)) {
                open_empty_cell(j, i);
            }
        }
    }
}


void Minesweeper::changeFlagState(int x, int y)
{
    cell current = field[y][x];
    current.flag = !current.flag;
    field[y][x] = current;
}