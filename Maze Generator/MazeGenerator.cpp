#include "MazeGenerator.h"
#include <random>
#include <vector>
#include <iostream>
#include <utility>

void MazeGenerator::generateMaze(Maze& maze) {
    int width = maze.getWidth();
    int height = maze.getHeight();

    vector<pair<int, int>> frontier;
    frontier.emplace_back(0, 0);
    maze.getCell(0, 0).visited = true;

    random_device rd;
    mt19937 gen(rd());

    while (!frontier.empty()) {
        uniform_int_distribution<> dist(0, frontier.size() - 1);
        pair<int, int> current = frontier[dist(gen)];
        int x = current.first;
        int y = current.second;
        frontier.erase(frontier.begin() + dist(gen));

        static const vector<pair<int, int>> directions = { {0, -1}, {1, 0}, {0, 1}, {-1, 0} };

        vector<pair<int, int>> neighbors;
        for (const auto& direction : directions) {
            int dx = direction.first;
            int dy = direction.second;
            int nx = x + dx;
            int ny = y + dy;

            if (nx >= 0 && ny >= 0 && nx < width && ny < height && !maze.getCell(nx, ny).visited) {
                neighbors.emplace_back(nx, ny);
            }
        }

        if (!neighbors.empty()) {
            uniform_int_distribution<> neighborDist(0, neighbors.size() - 1);
            pair<int, int> neighbor = neighbors[neighborDist(gen)];
            int nx = neighbor.first;
            int ny = neighbor.second;

            if (nx > x) {
                maze.getCell(x, y).rightWall = false;
                maze.getCell(nx, ny).leftWall = false;
            }
            else if (nx < x) {
                maze.getCell(x, y).leftWall = false;
                maze.getCell(nx, ny).rightWall = false;
            }
            else if (ny > y) {
                maze.getCell(x, y).bottomWall = false;
                maze.getCell(nx, ny).topWall = false;
            }
            else if (ny < y) {
                maze.getCell(x, y).topWall = false;
                maze.getCell(nx, ny).bottomWall = false;
            }

            maze.getCell(nx, ny).visited = true;
            frontier.emplace_back(nx, ny);
        }
    }
    cout << "Maze generated." << endl;
}
