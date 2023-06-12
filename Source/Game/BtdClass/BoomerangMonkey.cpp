#include "stdafx.h"
#include "BoomerangMonkey.h"

#include <valarray>

#include "Boomerang.h"

namespace Btd 
{
    BoomerangMonkey::BoomerangMonkey()
    {
        _range = 100;
        ThrowablePath = {
            "resources/towers/boomerang/Boomerang_1.bmp", "resources/towers/boomerang/Boomerang_2.bmp",
            "resources/towers/boomerang/Boomerang_3.bmp", "resources/towers/boomerang/Boomerang_4.bmp",
            "resources/towers/boomerang/Boomerang_5.bmp", "resources/towers/boomerang/Boomerang_6.bmp",
            "resources/towers/boomerang/Boomerang_7.bmp", "resources/towers/boomerang/Boomerang_8.bmp"
        };
        UpgradePrice[0] = 270;
        UpgradePrice[1] = 135;
        SetDamageType(DamageType::Normal);
    }

    void Btd::BoomerangMonkey::UpdateThrowable()
    {
        for (int i = static_cast<int>(throwables.size()) - 1; i >= 0; i--)
        {
            dynamic_pointer_cast<Boomerang>(throwables[i])->Update();
            if (Vector2Distance(throwables[i]->GetCenter(), GetCenter()) > static_cast<float>(_range) + 70)
            {
                // if throwable fly over (range + 70) distance will be erase
                throwables[i]->SetActive(false);
            }
            if (!throwables[i]->GetActive())
            {
                throwables.erase(throwables.begin() + i);
            }
        }
    }

    void Btd::BoomerangMonkey::Shoot(Vector2 target)
    {
        if (throwablePool.empty() || throwablePool.front()->GetActive())
        {
            PushThrowablePool();
        }
        auto BoomerangNext = dynamic_pointer_cast<Boomerang>(throwablePool.front());
        Vector2 targetDirection = {
            target.X - GetCenter().X, target.Y - GetCenter().Y
        };
        vector<Vector2> route = {{0, 0}, {500, 200}, {200, -600}, {0, 0}};
        const float angle = -atan2(-targetDirection.Y, targetDirection.X);//space angke transform
        const float scale = (float) _range/300.F; //origin route is use 300 range generate
        for (int i = 0; i < 4; i++)
        {
            route[i] = Spin(route[i], angle);
            route[i] = Scale(route[i],scale);
            route[i] = Vector2Add(GetCenter(), route[i]);
        }

        BoomerangNext->SetRoute(route);
        BoomerangNext->SetMaxExistTime(1000000);
        SetFrameIndexOfBitmap(GetFrameIndexByVector2(targetDirection));
        Tower::Shoot(target);
    }

    void Btd::BoomerangMonkey::PushThrowablePool()
    {
        auto boomerang = make_shared<Boomerang>(Boomerang());
        boomerang->LoadBitmapByString(ThrowablePath, RGB(255, 255, 255));
        boomerang->SetAnimation(30,false);
        boomerang->SetRoute({{100, 500}, {500, 0}, {0, 500}, {100, 500}});
        throwablePool.push(boomerang);
    }

    void BoomerangMonkey::Upgrade(int level)
    {
        Tower::Upgrade(level);
        switch (level)
        {
        case 0:
            SetMaxPop(4);
            break;
        case 1:
            SetDamageType(DamageType::SonicBoomerang);
            break;
        default:
            break;
        }
        IsUpgrade[level] = true;
    }
}
