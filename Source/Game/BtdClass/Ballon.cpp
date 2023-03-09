#include "BtdUtil.h"
#include "stdafx.h"
#include "Throwable.h"
#include "Ballon.h"

#include "map.h"

namespace Btd
{
    map<DamageType, bool> Ballon::resistDamegeMap =
    {
        {Normal, true},
        {Ice, true},
        {Boom, true}
    };

    void Ballon::SetNowRouteTarget(int target)
    {
        nowRouteTarget = target;
    }

    void Ballon::Setspeed(float speed)
    {
        _speed = speed;
    }

    void Ballon::Update()
    {
        if (GetActive())
        {
            //todo if pop (layer==0) setactive(false)
            Move(Map::GetRoute());
        }
    }

    float Ballon::GetSpeed()
    {
        return _speed;
    }

    void Ballon::Move(vector<Vector2> route)
    {
        Vector2 nowLocal;
        nowLocal.X = static_cast<float>(GetLeft());
        nowLocal.Y = static_cast<float>(GetTop());
        Vector2 target=route[nowRouteTarget];
        if (CompareVector2(target, nowLocal))
        {
            if (nowRouteTarget != static_cast<int>(route.size()) - 1)
                nowRouteTarget++;
            else
            {
                SetActive(false);
                //todo  arrive goal ,hp-layer
            }
        }
        target=route[nowRouteTarget];
        Vector2 deltaMove = Vector2Sub(target, nowLocal);
        Vector2 moveDirection = Normailize(deltaMove);
        float speed = GetSpeed();
        deltaMove = {moveDirection.X * speed, moveDirection.Y * speed};
        int left = GetLeft() + static_cast<int>(deltaMove.X);
        int top = GetTop() + static_cast<int>(deltaMove.Y);
        if(Vector2Distance({(float )left,(float )top},target)<1.5)
        {
            left=(int)target.X;
            top =(int) target.Y;
        }
        SetTopLeft(left, top);
    }
}
