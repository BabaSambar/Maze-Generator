#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <iostream>
#include <freeglut.h>

#include "Maze.h"
#include "MazeGenerator.h"
#include "Renderer.h"

Maze maze(10, 10);
Renderer renderer(maze);

void displayCallback() {
    renderer.drawMaze();
}

int main(int argc, char** argv) {
    MazeGenerator::generateMaze(maze);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Maze Generator with Bresenham Lines");

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);

    glutDisplayFunc(displayCallback);
    glutMainLoop();
    return 0;
}
