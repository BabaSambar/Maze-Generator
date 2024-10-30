#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include "Cell.h"

class Maze {
public:
    Maze(int width, int height);
    Cell& getCell(int x, int y);
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    std::pair<int, int> getEntry() const { return { 0, 0 }; }
    std::pair<int, int> getExit() const { return { width - 1, height - 1 }; }

private:
    int width, height;
    std::vector<std::vector<Cell>> grid;
};

#endif // MAZE_H
