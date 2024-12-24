#pragma once
#include <utility>
#include <windows.h>

class GraphicsEngine
{
public:
    static HWND hwnd;
    static HDC hdc;
public:
    static void CreateScreen();
    static bool DrawScreen();
    static void ClearScreen(HDC hdcMem);

    static void DrawSquare(HDC hdcMem, std::pair<int, int> position, COLORREF color);
    static void DrawRectangle(HDC hdcMem, const RECT& rect, COLORREF color);
    static void DrawSnake(HDC hdcMem);
    static void DrawFood(HDC hdcMem);
    static void DrawGrid(HDC hdcMem);
};