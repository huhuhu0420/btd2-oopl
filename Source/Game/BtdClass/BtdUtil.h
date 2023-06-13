﻿#pragma once
#include "../../Library/gameutil.h"
#include  <cmath>

namespace Btd
{
    struct Vector2
    {
        float X;
        float Y;
    };
    struct GameText
    {
        string text;
        Vector2 position;
        int color;
        int size;
    };

    enum class TowerType
    {
        dart,
        nail,
        ice,
        bomb,
        spikes,
        glue,
        boomerang,
        super,
    };

    enum class Layer
    {
        red = 0,
        blue,
        green,
        yellow,
        black,
        white,
        rainbow,
        lead,
    };

    enum class BtdSound
    {
        BACKGROUND,
        POP,
        LEAD,
        BOMB,
    };

    namespace BloonType
    {
        enum  BloonType
        {
            normal,
            black,
            white,
            rainbow,
            lead,
        };
    }

    enum class MapType
    {
        easy = 0,
        medium,
        hard
    };

    struct UnitRound
    {
        Layer type;
        int nextTime;
    };


    enum class DamageType
    {
        Normal,
        Boom,
        Ice,
        Glue,
        SonicBoomerang,
    };

    enum class GameFlow
    {
        Prepare,
        Shoot,
        Win,
        GameEnd
    };
    struct MonkeyBananaStats {
        Vector2 OriginPos = {-48763, 0};
        Vector2 PreMove = {0.0f, 0.0f};
        Vector2 SmoothMoving = {0.0f, 0.0f};
        bool Got🍌 = false;
        void* Target = nullptr;
        bool Tracing;
        float Speed;
    };
    Vector2 Normailize(float x, float y);
    Vector2 Normailize(Vector2);
    bool CompareVector2(Vector2 a, Vector2 b);
    int GetCursorPosX();
    int GetCursorPosY();
    Vector2 Vector2Add(Vector2 a, Vector2 b);
    Vector2 Vector2Sub(Vector2 a, Vector2 b);
    Vector2 Vector2MulFloat(Vector2 a, float n);
    float Vector2Distance(Vector2 a, Vector2 b);
    Vector2 Spin45(Vector2 a);
    Vector2 Spin45WithoutNormalize(Vector2 a);
    Vector2 Spin90(Vector2 a);
    int GetFrameIndexByVector2(Vector2 dir);
    Vector2 BezierCurve4Point(Vector2 a, Vector2 b, Vector2 c, Vector2 d, float n);
    Vector2 BezierCurve4Point(vector<Vector2> points, float n);
    Vector2 Spin(Vector2, float angle);
    Vector2 Scale(Vector2 v, float scale);
}
