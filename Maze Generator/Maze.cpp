#include "Maze.h"

Maze::Maze(int width, int height) : width(width), height(height), grid(width, std::vector<Cell>(height)) {}

Cell& Maze::getCell(int x, int y) {
    return grid[x][y];
}
