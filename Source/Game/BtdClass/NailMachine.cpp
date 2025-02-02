#include "stdafx.h"
#include "NailMachine.h"

#include "BloonFactory.h"

namespace Btd
{
    NailMachine::NailMachine()
    {
        _range = 120;
        shootDeltaTime = 5.5;
        ThrowablePath = {"resources/towers/nail/nail.bmp"};
        UpgradePrice[0] = 180;
        UpgradePrice[1] = 90;
        SetDamageType(DamageType::Normal);
    }

    void NailMachine::Update()
    {
        RangeCircle.SetCenter(static_cast<int>(GetCenter().X) - (_range - 100),
                              static_cast<int>(GetCenter().Y) - (_range - 100));
        if (_isActive)
        {
            UpdateThrowable();

            if (!BloonFactory::BloonVector.empty() && GetShootTimeCounter() > GetShootDeltaTime())
            {
                Bloon targetBloon = focus();
                Vector2 direction = {1, 0};
                if (Vector2Distance(GetCenter(), targetBloon.GetCenter()) < static_cast<float>(_range))
                {
                    for (int i = 0; i < 8; i++)
                    {
                        Vector2 target = {
                            GetCenter().X + direction.X,
                            GetCenter().Y + direction.Y
                        };
                        Shoot(target);
                        throwables.back()->SetSpeed(20);
                        direction = Spin45(direction);
                    }
                }
            }
            else
            {
                SetShootTimeCounter(GetShootTimeCounter() + deltaTime / 100.F);
            }
        }
    }

    void NailMachine::PushThrowablePool()
    {
        auto nail = make_shared<Throwable>(Throwable());
        nail->LoadBitmapByString(ThrowablePath, RGB(255, 255, 255));
        nail->SetSpeed(5);
        throwablePool.push(nail);
    }

    void NailMachine::Upgrade(int level)
    {
        Tower::Upgrade(level);
        
        switch (level)
        {
        case 0:
            shootDeltaTime = 3.5;
            break;
        case 1:
            _range = 160;
            break;
        default:
            break;
        }
        IsUpgrade[level] = true;
        
    }

}
