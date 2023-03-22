#include "stdafx.h"
#include "BtdUtil.h"

#include <complex>

#include "GameObject.h"

namespace Btd
{
    Vector2 Spin90(const Vector2 a)
    {
        return {-a.Y, a.X};
    }

    Vector2 Spin45(const Vector2 a)
    {
        Vector2 b = Spin90(a);
        Vector2 c = Vector2Add(a, b);
        return Normailize(c.X / 2, c.Y / 2);
    }

    Vector2 Normailize(float x, float y)
    {
        float powSum = sqrt(x * x + y * y);
        return {x / powSum, y / powSum};
    }

    Vector2 Normailize(Vector2 v)
    {
        float powSum = sqrt(v.X * v.X + v.Y * v.Y);
        return {v.X / powSum, v.Y / powSum};
    }

    bool CompareVector2(Vector2 a, Vector2 b)
    {
        return a.X == b.X && a.Y == b.Y;
    }

    Vector2 Vector2Add(Vector2 a, Vector2 b)
    {
        return {a.X + b.X, a.Y + b.Y};
    }

    Vector2 Vector2Sub(Vector2 a, Vector2 b)
    {
        return {a.X - b.X, a.Y - b.Y};
    }

    float Vector2Distance(Vector2 a, Vector2 b)
    {
        float x = a.X - b.X;
        float y = a.Y - b.Y;
        return sqrt(x * x + y * y);
    }

    int GetCursorPosX()
    {
        POINT p;
        GetCursorPos(&p);
        HWND hwnd = FindWindowA(nullptr, "Game");
        ScreenToClient(hwnd, &p);
        return static_cast<int>(p.x);
    }

    int GetCursorPosY()
    {
        POINT p;
        GetCursorPos(&p);
        HWND hwnd = FindWindowA(nullptr, "Game");
        ScreenToClient(hwnd, &p);
        return static_cast<int>(p.y);
    }

    bool isPointInBmp(POINT p, game_framework::CMovingBitmap target)
    {
        if (target.GetLeft() <= p.x && p.x <= target.GetLeft() + target.GetWidth() &&
            target.GetTop() <= p.y && p.y <= target.GetTop() + target.GetHeight())
        {
            return true;
        }
        return false;
    }
}
