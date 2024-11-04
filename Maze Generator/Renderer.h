#ifndef RENDERER_H
#define RENDERER_H
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <iostream>
#include <freeglut.h>
#include "Maze.h"

using namespace std;

class Renderer {
public:
    Renderer(Maze& maze);
    void drawMaze();

private:
    Maze& maze;
    void drawCell(int x, int y, bool isEntry, bool isExit);
    void drawBresenhamLine(int x1, int y1, int x2, int y2);
};

#endif // RENDERER_H
