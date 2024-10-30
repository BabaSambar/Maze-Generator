#ifndef CELL_H
#define CELL_H

class Cell {
public:
    bool topWall = true;
    bool rightWall = true;
    bool bottomWall = true;
    bool leftWall = true;
    bool visited = false;
};

#endif // CELL_H
