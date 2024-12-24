#include "SnakeGame.h"

#include <chrono>
#include <ctime>

#include "GraphicsEngine.h"
#include "Snake.h"

Snake *SnakeGame::snake;
std::pair<int, int> SnakeGame::foodPosition;
int SnakeGame::GAME_SIZE = 16;
float SnakeGame::GAME_SPEED = 0.2f;

using namespace std;

int main(int argc, char* argv[])
{
    SnakeGame::foodPosition = make_pair(-1, -1);
    srand(time(NULL));
    SnakeGame::snake = new Snake(RGB(200, 80, 80));
    GraphicsEngine::CreateScreen();

    const chrono::milliseconds frameDuration(1000 / 60);
    auto start_time = chrono::high_resolution_clock::now();
    
    while (true)
    {
        //auto frame_start = chrono::high_resolution_clock::now();
        
        if (!GraphicsEngine::DrawScreen())
            break;

        auto current_time = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = current_time - start_time;

        if (elapsed.count() > SnakeGame::GAME_SPEED)
        {
            SnakeGame::snake->Move();
            start_time = chrono::high_resolution_clock::now();
        }

        SnakeGame::DetectEatFood();
        SnakeGame::GenerateFood();
    }
    
    return 0;
}

void SnakeGame::GenerateFood()
{
    while (foodPosition.first == -1)
    {
        pair<int, int> nextPosition = make_pair(rand() % SnakeGame::GAME_SIZE, rand() % SnakeGame::GAME_SIZE);

        if (!snake->IsSnakeAtPosition(nextPosition))
            foodPosition = nextPosition;
    }
}

void SnakeGame::DetectEatFood()
{
    if (snake->IsSnakeAtPosition(foodPosition))
    {
        snake->EatFood();
        foodPosition = make_pair(-1, -1);
    }
}
