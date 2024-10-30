#include "Renderer.h"
#include <cmath>

Renderer::Renderer(Maze& maze) : maze(maze) {}

void Renderer::drawMaze() {
    glClear(GL_COLOR_BUFFER_BIT);
    int width = maze.getWidth();
    int height = maze.getHeight();

    // Get entry and exit points
    std::pair<int, int> entry = maze.getEntry();
    int entryX = entry.first;
    int entryY = entry.second;

    std::pair<int, int> exit = maze.getExit();
    int exitX = exit.first;
    int exitY = exit.second;

    // Draw each cell with walls, highlighting entry and exit
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            bool isEntry = (x == entryX && y == entryY);
            bool isExit = (x == exitX && y == exitY);
            drawCell(x, y, isEntry, isExit);
        }
    }
    glFlush();
}

void Renderer::drawCell(int x, int y, bool isEntry, bool isExit) {
    Cell& cell = maze.getCell(x, y);
    float cellSize = 1.0f / maze.getWidth();
    float startX = x * cellSize;
    float startY = y * cellSize;

    if (isEntry) {
        glColor3f(0.0f, 1.0f, 0.0f);
    }
    else if (isExit) {
        glColor3f(1.0f, 0.0f, 0.0f);
    }
    else {
        glColor3f(0.0f, 0.0f, 0.0f);
    }

    if (cell.topWall) drawBresenhamLine(startX, startY + cellSize, startX + cellSize, startY + cellSize);
    if (cell.rightWall) drawBresenhamLine(startX + cellSize, startY, startX + cellSize, startY + cellSize);
    if (cell.bottomWall) drawBresenhamLine(startX, startY, startX + cellSize, startY);
    if (cell.leftWall) drawBresenhamLine(startX, startY, startX, startY + cellSize);
}

void Renderer::drawBresenhamLine(float x1, float y1, float x2, float y2) {
    int dx = std::abs(x2 - x1), dy = std::abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1, sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    glBegin(GL_POINTS);
    while (true) {
        glVertex2f(x1, y1);
        if (x1 == x2 && y1 == y2) break;

        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
    glEnd();
}
