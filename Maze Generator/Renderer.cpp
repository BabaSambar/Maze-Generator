#include "Renderer.h"
#include <cmath>

Renderer::Renderer(Maze& maze) : maze(maze) {}

void Renderer::drawMaze() {
    glClear(GL_COLOR_BUFFER_BIT);
    int width = maze.getWidth();
    int height = maze.getHeight();

    // Get entry and exit points
    pair<int, int> entry = maze.getEntry();
    int entryX = entry.first;
    int entryY = entry.second;

    pair<int, int> exit = maze.getExit();
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

    startX *= 100;
    startY *= 100;

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

    cout << "Cell drawn at " << startX << ", " << startY << "\n";
}

float roundFloat(float x)
{
    return (int)(x + 0.5);
}

void Renderer::drawBresenhamLine(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1, dy = y2 - y1, steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    int xinc = dx / (float)steps, yinc = dy / (float)steps;
    float x = x1, y = y1;

    glBegin(GL_POINTS);
    glVertex2d(x1, y1);
    for (int i = 0; i < steps; i++)
    {
        glVertex2d(roundFloat(x), roundFloat(y));
        x += xinc;
        y += yinc;
        glEnd();
        glFlush();
        Sleep(5);
        glBegin(GL_POINTS);
    }
    glEnd();
    glFlush();
}
