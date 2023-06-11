#include "mazeGenerator.h"

#include <algorithm>
#include <ctime>
#include <stack>
#include <vector>
#include <chrono>
#include <random>



MazeGenerator::MazeGenerator(int width, int height, const Level::LevelElement& kLevelElement)
    : width(width * 2 + 1), height(height * 2 + 1),
    maze(this->width, std::vector<int>(this->height, 1)) {

    do {
        DFS(kLevelElement);
    } while (!isConnected());
}


int MazeGenerator::operator()(int i, int j) const {
    return maze[i][j];
}

int MazeGenerator::getWidth() const {
    return width;
}

int MazeGenerator::getHeight() const {
    return height;
}

void MazeGenerator::DFS(const Level::LevelElement& kLevelElement) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, kLevelElement.mazeLevel);

    std::stack<std::pair<int, int>> st;
    st.push({ 2, 2 });

    std::vector<std::pair<int, int>> dirs = { {-2, 0}, {2, 0}, {0, -2}, {0, 2} };

    while (!st.empty()) {
        std::pair<int, int> current = st.top();
        st.pop();
        int x = current.first;
        int y = current.second;
        maze[x][y] = 0;

        std::shuffle(dirs.begin(), dirs.end(), gen);

        // Keep track of valid next cells.
        std::vector<std::pair<int, int>> validNext;

        for (auto dir : dirs) {
            int nx = x + dir.first;
            int ny = y + dir.second;
            int mx = (nx + x) / 2;
            int my = (ny + y) / 2;

            if (nx >= 2 && nx < width - 1 && ny >= 2 && ny < height - 1 && maze[nx][ny] == 1) {
                validNext.push_back({ nx, ny });
                maze[mx][my] = 0;
            }
        }

        // Randomly select a next cell and randomly decide whether to create dead-ends.
        if (!validNext.empty()) {
            std::shuffle(validNext.begin(), validNext.end(), gen);
            for (size_t i = 0; i < validNext.size(); ++i) {
                if (i == 0 || dis(gen) < 3) {  // Here we use a random variable to decide whether to create a dead-end.
                    // The first one is the next cell to visit, or if the random variable is less than 3, create another path.
                    st.push(validNext[i]);
                }
                else {
                    // Others are dead-ends.
                    maze[validNext[i].first][validNext[i].second] = 0;
                }
            }
        }
    }
}

bool MazeGenerator::isConnected() {
    std::vector<std::vector<bool>> visited(width, std::vector<bool>(height, false));
    std::stack<std::pair<int, int>> st;
    st.push({ 2, 2 });

    while (!st.empty()) {
        std::pair<int, int> current = st.top();
        st.pop();
        int x = current.first;
        int y = current.second;

        if (!visited[x][y]) {
            visited[x][y] = true;

            std::vector<std::pair<int, int>> dirs = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

            for (auto dir : dirs) {
                int nx = x + dir.first;
                int ny = y + dir.second;

                if (nx >= 0 && nx < width && ny >= 0 && ny < height && maze[nx][ny] == 0 && !visited[nx][ny]) {
                    st.push({ nx, ny });
                }
            }
        }
    }

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            if (maze[i][j] == 0 && !visited[i][j]) {
                return false;  // Found an unvisited empty space, the maze is not connected.
            }
        }
    }

    return true;  // All empty spaces are visited, the maze is connected.
}
