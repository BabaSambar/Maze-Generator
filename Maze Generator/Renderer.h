#ifndef RENDERER_H
#define RENDERER_H
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <iostream>
#include <freeglut.h>
#include "Maze.h"

class Renderer {
public:
    Renderer(Maze& maze);
    void drawMaze();

private:
    Maze& maze;
    void drawCell(int x, int y, bool isEntry, bool isExit);
    void drawBresenhamLine(float x1, float y1, float x2, float y2);
};

#endif // RENDERER_H
