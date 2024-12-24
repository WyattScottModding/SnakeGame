#include "Snake.h"

#include "SnakeGame.h"

using namespace std;

Snake::Snake(COLORREF color)
{
    this->color = color;
    ResetSnake();
}

void Snake::ResetSnake()
{
    this->positions.clear();
    this->positions.push_back(make_pair(0, 0));
    this->direction = make_pair(1, 0);
}

bool Snake::IsSnakeAtPosition(std::pair<int, int> position)
{
    for (int i = 0; i < positions.size(); i++)
    {
        if (positions[i].first == position.first && positions[i].second == position.second)
            return true;
    }

    return false;
}

void Snake::MoveUp()
{
    direction = make_pair(0, -1);
}

void Snake::MoveDown()
{
    direction = make_pair(0, 1);
}

void Snake::MoveRight()
{
    direction = make_pair(1, 0);
}

void Snake::MoveLeft()
{
    direction = make_pair(-1, 0);
}

void Snake::Move()
{
    prevHeadPosition = positions[0];
    prevTailPosition = positions[positions.size() - 1];
    
    for (int i = positions.size() - 1; i > 0; i--)
    {
        positions[i] = positions[i - 1];
    }

    positions[0] = make_pair(positions[0].first + direction.first, positions[0].second + direction.second);

    if (CheckForDeath())
    {
        ResetSnake();
    }
}

void Snake::EatFood()
{
    positions.push_back(prevTailPosition);
}

bool Snake::CheckForDeath()
{
    // Check for out of bounds
    if (positions[0].first < 0 || positions[0].first >= SnakeGame::GAME_SIZE || positions[0].second < 0 || positions[0].second >= SnakeGame::GAME_SIZE)
    {
        return true;
    }

    // Snake ate itself
    for (int i = 1; i < positions.size(); i++)
    {
        if (positions[0].first == positions[i].first && positions[0].second == positions[i].second)
            return true;
    }

    return false;
}
