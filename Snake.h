#pragma once
#include <vector>

#include "GraphicsEngine.h"

class Snake
{
public:
    std::vector<std::pair<int, int>> positions;
    std::pair<int, int> direction;
    COLORREF color;

    std::pair<int, int> prevHeadPosition;
    std::pair<int, int> prevTailPosition;
public:
    Snake(COLORREF color);
    void ResetSnake();
    bool IsSnakeAtPosition(std::pair<int, int> position);

    void MoveUp();
    void MoveDown();
    void MoveRight();
    void MoveLeft();

    void Move();

    void EatFood();
    bool CheckForDeath();
};
