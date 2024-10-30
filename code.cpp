#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

struct Cell {
    bool topWall = true;
    bool rightWall = true;
    bool visited = false;
};

class Maze {
public:
    Maze(int size) : size(size) {
        grid.resize(size, std::vector<Cell>(size));
        generateMaze(0, 0);
    }

    void generateMaze(int x, int y) {
        grid[x][y].visited = true;
        
        std::vector<std::pair<int, int>> directions = {{0,-1}, {1,0}, {0,1}, {-1,0}};
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(directions.begin(), directions.end(), gen);

        for (auto [dx, dy] : directions) {
            int newX = x + dx;
            int newY = y + dy;
            
            if (newX >= 0 && newX < size && newY >= 0 && newY < size && !grid[newX][newY].visited) {
                if (dx == 1) grid[x][y].rightWall = false;
                else if (dx == -1) grid[newX][newY].rightWall = false;
                if (dy == -1) grid[newX][newY].topWall = false;
                else if (dy == 1) grid[x][y].topWall = false;
                
                generateMaze(newX, newY);
            }
        }
    }

    bool canMove(int x, int y, int dx, int dy) const {
        if (x + dx < 0 || x + dx >= size || y + dy < 0 || y + dy >= size) return false;
        
        if (dx == 1 && grid[x][y].rightWall) return false;
        if (dx == -1 && grid[x+dx][y].rightWall) return false;
        if (dy == 1 && grid[x][y].topWall) return false;
        if (dy == -1 && grid[x][y+dy].topWall) return false;
        
        return true;
    }

    int getSize() const { return size; }
    const std::vector<std::vector<Cell>>& getGrid() const { return grid; }

private:
    int size;
    std::vector<std::vector<Cell>> grid;
};

class Game {
public:
    Game() : maze(10), playerX(0), playerY(0) {
        std::cout << "Game started! Use WASD to move, R to reset, ESC to exit" << std::endl;
    }

    void draw() {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        
        // Draw maze
        glColor3f(0.0f, 0.0f, 0.0f);
        float cellSize = 1.8f / maze.getSize();
        glTranslatef(-0.9f, -0.9f, 0.0f);

        for (int x = 0; x < maze.getSize(); x++) {
            for (int y = 0; y < maze.getSize(); y++) {
                float startX = x * cellSize;
                float startY = y * cellSize;
                
                glBegin(GL_LINES);
                if (maze.getGrid()[x][y].topWall) {
                    glVertex2f(startX, startY + cellSize);
                    glVertex2f(startX + cellSize, startY + cellSize);
                }
                if (maze.getGrid()[x][y].rightWall) {
                    glVertex2f(startX + cellSize, startY);
                    glVertex2f(startX + cellSize, startY + cellSize);
                }
                if (y == 0) {
                    glVertex2f(startX, startY);
                    glVertex2f(startX + cellSize, startY);
                }
                if (x == 0) {
                    glVertex2f(startX, startY);
                    glVertex2f(startX, startY + cellSize);
                }
                glEnd();
            }
        }

        // Draw player
        glColor3f(1.0f, 0.0f, 0.0f);
        float playerPosX = playerX * cellSize + cellSize/2;
        float playerPosY = playerY * cellSize + cellSize/2;
        float playerSize = cellSize/3;
        
        glBegin(GL_QUADS);
        glVertex2f(playerPosX - playerSize, playerPosY - playerSize);
        glVertex2f(playerPosX + playerSize, playerPosY - playerSize);
        glVertex2f(playerPosX + playerSize, playerPosY + playerSize);
        glVertex2f(playerPosX - playerSize, playerPosY + playerSize);
        glEnd();

        // Draw goal
        glColor3f(0.0f, 1.0f, 0.0f);
        float goalX = (maze.getSize()-1) * cellSize + cellSize/2;
        float goalY = (maze.getSize()-1) * cellSize + cellSize/2;
        
        glBegin(GL_QUADS);
        glVertex2f(goalX - playerSize, goalY - playerSize);
        glVertex2f(goalX + playerSize, goalY - playerSize);
        glVertex2f(goalX + playerSize, goalY + playerSize);
        glVertex2f(goalX - playerSize, goalY + playerSize);
        glEnd();

        glutSwapBuffers();
    }

    void handleKey(unsigned char key) {
        int dx = 0, dy = 0;
        
        switch(key) {
            case 'w': case 'W': dy = 1; break;
            case 's': case 'S': dy = -1; break;
            case 'a': case 'A': dx = -1; break;
            case 'd': case 'D': dx = 1; break;
            case 'r': case 'R':
                maze = Maze(10);
                playerX = playerY = 0;
                std::cout << "Game reset!" << std::endl;
                break;
            case 27: // ESC
                exit(0);
                break;
        }

        if (dx != 0 || dy != 0) {
            if (maze.canMove(playerX, playerY, dx, dy)) {
                playerX += dx;
                playerY += dy;
                std::cout << "Player moved to position (" << playerX << "," << playerY << ")" << std::endl;
                
                if (playerX == maze.getSize()-1 && playerY == maze.getSize()-1) {
                    std::cout << "Congratulations! You reached the goal!" << std::endl;
                }
            }
        }
        
        glutPostRedisplay();
    }

private:
    Maze maze;
    int playerX, playerY;
};

Game* game;

void display() {
    game->draw();
}

void keyboard(unsigned char key, int x, int y) {
    game->handleKey(key);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Simple Maze Game");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    game = new Game();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    
    std::cout << "\nGame Instructions:" << std::endl;
    std::cout << "- Use WASD keys to move the red square" << std::endl;
    std::cout << "- Try to reach the green square" << std::endl;
    std::cout << "- Press R to reset the game" << std::endl;
    std::cout << "- Press ESC to exit" << std::endl;

    glutMainLoop();
    delete game;
    return 0;
}
