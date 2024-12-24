#include "GraphicsEngine.h"

#include <iostream>
#include <ostream>

#include "SnakeGame.h"

HWND GraphicsEngine::hwnd;
HDC GraphicsEngine::hdc;

const int SCREEN_WIDTH = 600;

float SquareWidth;
float SquareHeight;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);

                EndPaint(hwnd, &ps);
                return 0;
            }
        case WM_KEYDOWN:
            {
                if (wParam == VK_ESCAPE)
                {
                    PostQuitMessage(0);
                    return 0;
                }
                if (wParam == 'W')
                {
                    SnakeGame::snake->MoveUp();
                }
                else if (wParam == 'A')
                {
                    SnakeGame::snake->MoveLeft();
                }
                else if (wParam == 'S')
                {
                    SnakeGame::snake->MoveDown();
                }
                else if (wParam == 'D')
                {
                    SnakeGame::snake->MoveRight();
                }
            }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void GraphicsEngine::CreateScreen()
{
    const wchar_t* className = L"GraphicsEngine";
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = className;

    RegisterClass(&wc);

    DWORD windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX;


    GraphicsEngine::hwnd = CreateWindowEx(0, className, L"Snake Game", windowStyle,
        CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH, SCREEN_WIDTH, nullptr, nullptr,
        wc.hInstance, nullptr);

    if (hwnd == nullptr)
        std::cout << "Error creating window" << std::endl;

    ShowWindow(hwnd, SW_SHOW);

    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    
    SquareWidth = static_cast<float>(clientRect.right) / SnakeGame::GAME_SIZE;
    SquareHeight = static_cast<float>(clientRect.bottom) / SnakeGame::GAME_SIZE;
}

bool GraphicsEngine::DrawScreen()
{
    MSG msg = {};
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            return false;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    hdc = GetDC(hwnd);

    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, SCREEN_WIDTH, SCREEN_WIDTH);
    HGDIOBJ oldBitmap = SelectObject(hdcMem, hBitmap);

    ClearScreen(hdcMem);
    DrawSnake(hdcMem);
    DrawFood(hdcMem);
    DrawGrid(hdcMem);

    BitBlt(hdc, 0, 0, SCREEN_WIDTH, SCREEN_WIDTH, hdcMem, 0, 0, SRCCOPY);

    SelectObject(hdcMem, oldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(hwnd, hdc);
    

    return true;
}

void GraphicsEngine::ClearScreen(HDC hdcMem)
{
    RECT rect;
    GetClientRect(hwnd, &rect);
    FillRect(hdcMem, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
}

void GraphicsEngine::DrawSquare(HDC hdcMem, std::pair<int, int> position, COLORREF color)
{
    LONG left = static_cast<LONG>(position.first * SquareWidth);
    LONG top = static_cast<LONG>(position.second * SquareHeight);
    LONG right = static_cast<LONG>((position.first + 1) * SquareWidth);
    LONG bottom = static_cast<LONG>((position.second + 1) * SquareHeight);

    RECT rect = { left, top, right, bottom };

    SetDCBrushColor(hdcMem, color);
    FillRect(hdcMem, &rect, (HBRUSH)GetStockObject(DC_BRUSH));
}

void GraphicsEngine::DrawRectangle(HDC hdcMem, const RECT& rect, COLORREF color)
{
    SetDCBrushColor(hdcMem, color);
    FillRect(hdcMem, &rect, (HBRUSH)GetStockObject(DC_BRUSH));
}

void GraphicsEngine::DrawSnake(HDC hdcMem)
{
    for (const std::pair<int, int> position : SnakeGame::snake->positions)
    {
        DrawSquare(hdcMem, position, SnakeGame::snake->color);
    }
}

void GraphicsEngine::DrawFood(HDC hdcMem)
{
    DrawSquare(hdcMem, SnakeGame::foodPosition, RGB(20, 200, 20));
}

void GraphicsEngine::DrawGrid(HDC hdcMem)
{
    // Vertical lines
    for (int i = 0; i <= SnakeGame::GAME_SIZE; i++)
    {
        LONG xPos = static_cast<LONG>(i * SquareWidth);

        RECT line = { xPos, 0, xPos + 1, static_cast<LONG>(SCREEN_WIDTH)};
        DrawRectangle(hdcMem, line, RGB(0, 0, 0));
    }

    // Horizontal lines
    for (int i = 0; i <= SnakeGame::GAME_SIZE; i++)
    {
        LONG yPos = static_cast<LONG>(i * SquareHeight);

        RECT line = {0, yPos, static_cast<LONG>(SCREEN_WIDTH), yPos + 1};
        DrawRectangle(hdcMem, line, RGB(0, 0, 0));
    }
}
