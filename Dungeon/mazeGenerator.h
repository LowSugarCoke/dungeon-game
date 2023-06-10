#pragma once
#include <vector>

#include "level.h"

class MazeGenerator {
public:
    MazeGenerator(int width, int height, const Level::LevelElement& kLevelElement);

    int operator()(int i, int j) const;
    int getWidth() const;
    int getHeight() const;

private:
    int width;
    int height;
    std::vector<std::vector<int>> maze;

    void DFS(const Level::LevelElement& kLevelElement);
};