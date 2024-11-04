#include <vector>
#include <random>
#include <algorithm>

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <iostream>
#include <freeglut.h>

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

        std::vector<std::pair<int, int>> directions = { {0, -1}, {1, 0}, {0, 1}, {-1, 0} };
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(directions.begin(), directions.end(), gen);

        for (const auto& direction : directions) {
            int dx = direction.first;
            int dy = direction.second;
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
        if (dx == -1 && grid[x + dx][y].rightWall) return false;
        if (dy == 1 && grid[x][y].topWall) return false;
        if (dy == -1 && grid[x][y + dy].topWall) return false;

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
    enum class GameState { START, PLAY, END };

    Game() : maze(10), playerX(0), playerY(0), state(GameState::START) {
        std::cout << "Press ENTER to start the game!" << std::endl;
    }

    void draw() {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();

        switch (state) {
        case GameState::START:
            drawStartScreen();
            break;
        case GameState::PLAY:
            drawMaze();
            break;
        case GameState::END:
            drawEndScreen();
            break;
        }

        glutSwapBuffers();
    }

    void handleKey(unsigned char key) {
        if (state == GameState::START && (key == 13)) { // ENTER key
            state = GameState::PLAY;
            playerX = playerY = 0;
            std::cout << "Game started! Use WASD to move, R to reset, ESC to exit" << std::endl;
        }
        else if (state == GameState::PLAY) {
            int dx = 0, dy = 0;

            switch (key) {
            case 'w': case 'W': dy = 1; break;
            case 's': case 'S': dy = -1; break;
            case 'a': case 'A': dx = -1; break;
            case 'd': case 'D': dx = 1; break;
            case 'r': case 'R':
                resetGame();
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

                    if (playerX == maze.getSize() - 1 && playerY == maze.getSize() - 1) {
                        state = GameState::END;
                        std::cout << "Congratulations! You reached the goal!" << std::endl;
                    }
                }
            }
        }
        else if (state == GameState::END && (key == 'r' || key == 'R')) {
            resetGame();
        }

        glutPostRedisplay();
    }

private:
    Maze maze;
    int playerX, playerY;
    GameState state;

    void resetGame() {
        maze = Maze(10);
        playerX = playerY = 0;
        state = GameState::PLAY;
        std::cout << "Game reset!" << std::endl;
    }

    void drawMaze() {
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
        float playerPosX = playerX * cellSize + cellSize / 2;
        float playerPosY = playerY * cellSize + cellSize / 2;
        float playerSize = cellSize / 3;

        glBegin(GL_QUADS);
        glVertex2f(playerPosX - playerSize, playerPosY - playerSize);
        glVertex2f(playerPosX + playerSize, playerPosY - playerSize);
        glVertex2f(playerPosX + playerSize, playerPosY + playerSize);
        glVertex2f(playerPosX - playerSize, playerPosY + playerSize);
        glEnd();

        // Draw goal
        glColor3f(0.0f, 1.0f, 0.0f);
        float goalX = (maze.getSize() - 1) * cellSize + cellSize / 2;
        float goalY = (maze.getSize() - 1) * cellSize + cellSize / 2;

        glBegin(GL_QUADS);
        glVertex2f(goalX - playerSize, goalY - playerSize);
        glVertex2f(goalX + playerSize, goalY - playerSize);
        glVertex2f(goalX + playerSize, goalY + playerSize);
        glVertex2f(goalX - playerSize, goalY + playerSize);
        glEnd();
    }

    void drawStartScreen() {
        glColor3f(0.0f, 0.0f, 0.0f);

        // Center each line of text by adjusting the raster position
        glRasterPos2f(-0.15f, 0.1f); // Adjust this for each line's position
        const char* text1 = "ENTER TO START";
        for (const char* c = text1; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }

        glRasterPos2f(-0.1f, 0.0f);
        const char* text2 = "R TO RESET";
        for (const char* c = text2; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }

        glRasterPos2f(-0.12f, -0.1f);
        const char* text3 = "WASD TO MOVE";
        for (const char* c = text3; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
    }


    void drawEndScreen() {
        glColor3f(0.0f, 0.0f, 1.0f);
        glRasterPos2f(-0.15f, 0.0f);
        const char* text = "Congratulations! Press R to Restart";
        for (const char* c = text; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
    }
};

Game* game;

void display() {
    game->draw();
}

void keyboard(unsigned char key, int, int) {
    game->handleKey(key);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Maze Game");

    game = new Game();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    delete game;
    return 0;
}
