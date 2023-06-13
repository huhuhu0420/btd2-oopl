﻿#include "stdafx.h"
#include "BloonFactory.h"
#include "BtdUtil.h"
#include "map.h"
#include "Tower.h"
#include "Cavallo.h"

#include "BoomerangMonkey.h"

namespace Btd
{
    Tower::Tower()
    {
        IsUpgrade[0] = false;
        IsUpgrade[1] = false;
        _isMovable = true;
        _isClicked = true;
        ThrowablePath = {"resources/towers/bomb/bomb.bmp"};
        shootTimecounter = 0;
        InitUpgradeBtn();
        _maxPop=1;
        _throwableOffset={0,0};
    }

    bool Tower::IsMovable()
    {
        return _isMovable;
    }

    void Tower::SetIsMove(bool move)
    {
        _isMovable = move;
    }

    int Tower::GetRange()
    {
        return _range;
    }

    void Tower::UpdateThrowable()
    {
        for (int i = static_cast<int>(throwables.size()) - 1; i >= 0; i--)
        {
            throwables[i]->Update();
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

    Bloon Tower::focus()
    {
        Bloon target = BloonFactory::BloonVector[0];
        // first 'for' can detect the first bloon in range
        // second 'for' decide the proper bloon to shoot
        for (int i = 0; i < static_cast<int>(BloonFactory::BloonVector.size()); i++)
        {
            if (Vector2Distance(BloonFactory::BloonVector[i].GetCenter(), this->GetCenter()) <= static_cast<float>(
                _range))
            {
                target = BloonFactory::BloonVector[i];
            }
        }
        for (int i = 0; i < static_cast<int>(BloonFactory::BloonVector.size()); i++)
        {
            if (Vector2Distance(BloonFactory::BloonVector[i].GetCenter(), this->GetCenter()) <= static_cast<float>(
                _range))
            {
                auto b = BloonFactory::BloonVector[i];
                if ((BloonFactory::BloonVector[i].GetNowRouteTarget() > target.GetNowRouteTarget()) ||
                    (BloonFactory::BloonVector[i].GetNowRouteTarget() == target.GetNowRouteTarget() &&
                        Vector2Distance(BloonFactory::BloonVector[i].GetCenter(),
                                        Map::GetRoute()[b.Getroute()][b.GetNowRouteTarget()]) <
                        Vector2Distance(target.GetCenter(),
                                        Map::GetRoute()[b.Getroute()][b.GetNowRouteTarget()])))
                {
                    target = BloonFactory::BloonVector[i];
                }
            }
        }
        return target;
    }

    void Tower::HandleUpgradeBtnFrame(int money)
    {
        for (int i=0; i<2; i++)
        {
            if (IsUpgrade[i])
            // already bought
            {
                UpgradeBtn[i].SetFrameIndexOfBitmap(2);
            }
            else if (UpgradePrice[i] > money)
            // can't buy
            {
                UpgradeBtn[i].SetFrameIndexOfBitmap(0);
            }
            else
            // not buy
            {
                UpgradeBtn[i].SetFrameIndexOfBitmap(1);
            }
        }
    }

    void Tower::SetThrowableOffset(Vector2 offset)
    {
        _throwableOffset = offset;
    }

    Vector2 Tower::GetThrowableOffset()
    {
        return _throwableOffset;
    }

    void Tower::SetDamageType(DamageType damageType)
    {
        _damageType = damageType;
    }

    void Tower::SetMoney(int price)
    {
        _money = price;
    }

    int Tower::GetMoney()
    {
        return _money;
    }

    void Tower::SetShootTimeCounter(float tome)
    {
        shootTimecounter = tome;
    }

    void Tower::InitUpgradeBtn()
    {
        int location[2][2] = {{750, 342}, {860, 342}};
        for (int i=0; i<2; i++)
        {
            UpgradeBtn[i].LoadBitmapByString({"resources/button/cantBuy.bmp", "resources/button/notBuy.bmp"
            , "resources/button/alreadyBought.bmp"});
            UpgradeBtn[i].SetTopLeft(location[i][0], location[i][1]);
        }
        SellBtn.LoadBitmapByString({"resources/button/sell.bmp"});
        SellBtn.SetTopLeft(753, 563);
    }

    void Tower::Upgrade(int level)
    {
        if (!IsUpgrade[level])
        {
            _money+=UpgradePrice[level];
        }
    }

    void Tower::TowerShow()
    {
        if (_isClicked)
        {
            this->RangeCircle.ShowBitmap(static_cast<float>(_range) / 100.0);
            if (!_isMovable)
            {
                this->UpgradeBtn[0].ShowBitmap();
                this->UpgradeText[0].ShowBitmap();
                this->UpgradeBtn[1].ShowBitmap();
                this->UpgradeText[1].ShowBitmap();
                this->SellBtn.ShowBitmap();
            }
        }
        for (int i = 0; i < static_cast<int>(throwables.size()); i++)
        {
            throwables[i]->ShowBitmap();
        }
        this->ShowBitmap();
    }

    float Tower::GetShootDeltaTime()
    {
        return shootDeltaTime;
    }

    float Tower::GetShootTimeCounter()
    {
        return shootTimecounter;
    }

    void Tower::Update()
    {
        if (_isMovable || Cavallo::CAVALLO)
        {
            RangeCircle.SetCenter(static_cast<int>(GetCenter().X) - (_range - 100),
                                  static_cast<int>(GetCenter().Y) - (_range - 100));
        }
        if (_isActive)
        {
            UpdateThrowable();

            if (!BloonFactory::BloonVector.empty() && shootTimecounter > shootDeltaTime)
            {
                Bloon target = focus();
                if (Vector2Distance(GetCenter(), target.GetCenter()) < static_cast<float>(_range))
                {
                    Shoot({(target.GetCenter().X), (target.GetCenter().Y)});
                }
            }
            else
            {
                shootTimecounter += static_cast<float>(delayCount) / 100.F;
            }
        }
    }

    void Tower::SetShootDeltaTime(float time)
    {
        shootDeltaTime = time;
    }

    void Tower::Shoot(Vector2 target)
    {
        shootTimecounter = 0;
        if (throwablePool.empty() || throwablePool.front()->GetActive())
        {
            PushThrowablePool();
        }
        auto next = throwablePool.front();
        Vector2 targetDirection = {
            target.X - GetCenter().X, target.Y - GetCenter().Y
        };
        throwablePool.pop();
        next->SetActive(true);
        next->SetDamageType(_damageType);
        auto offset = _throwableOffset;
        auto offsetSpinTimes = GetFrameIndexByVector2(targetDirection);
        for(int i =0;i<offsetSpinTimes;i++)
        {
            offset = Spin45WithoutNormalize(offset);
        }
        next->InitByCenter(Vector2Add(GetCenter(),offset));
        next->SetMoveDirection(targetDirection.X, targetDirection.Y);
        next->SetMaxPop(_maxPop);
        throwables.push_back(next);
    }

    void Tower::SetMaxPop(int i )
    {
        _maxPop=i;
    }
    void Tower::SetThrowablePath(vector<string> name)
    {
        ThrowablePath = name;
    }
    void Tower::SetOriginPos(Vector2 pos)
    {
        🐒🍌Stats.OriginPos = pos;
	}
    // it is throwable factory
    void Tower::PushThrowablePool()
    {
        auto tmp = make_shared<Throwable>(Throwable());
        tmp->LoadBitmapByString({"resources/towers/bomb/bomb.bmp"}
                                ,RGB(255, 255, 255));
        throwablePool.push(tmp);
    }
    bool Tower::Yes🍌😄(Cavallo::Banana* 🍌) {
        // go to the 🍌
        if (🐒🍌Stats.Target != 🍌 && !IsOverlap(*this, *🍌)) {
            🐒🍌Stats.Got🍌 = false;
            🐒🍌Stats.Target = 🍌;
        }
        static random_device rd;
        static mt19937 gen(rd());
        static uniform_real_distribution<float> dis(🐵Minspeed * Cavallo::Multiplier, 🐵Maxspeed * Cavallo::Multiplier);
        🐒🍌Stats.Speed = dis(gen) ;
        if (🍌->GotCarry() && !🐒🍌Stats.Got🍌) return No🍌😭();
        Vector2 VecMove = 🐒🍌Stats.Got🍌 ? 🐒🍌Stats.PreMove : Normailize({🍌->GetCenter().X - GetCenter().X, 🍌->GetCenter().Y - GetCenter().Y});
        SetFrameIndexOfBitmap(min(GetFrameSizeOfBitmap() - 1 , GetFrameIndexByVector2(VecMove)));
        🐒🍌Stats.PreMove = VecMove;
        VecMove.X = VecMove.X * 🐒🍌Stats.Speed + GetCenter().X;
        VecMove.Y = VecMove.Y * 🐒🍌Stats.Speed + GetCenter().Y;
        int x = static_cast<int>(VecMove.X);
        int y = static_cast<int>(VecMove.Y);
        🐒🍌Stats.SmoothMoving.X += VecMove.X - x;
        🐒🍌Stats.SmoothMoving.Y += VecMove.Y - y;
        if (fabs(🐒🍌Stats.SmoothMoving.X) >= 1.0f)
        {
			x += static_cast<int>(🐒🍌Stats.SmoothMoving.X);
            🐒🍌Stats.SmoothMoving.X -= static_cast<int>(🐒🍌Stats.SmoothMoving.X);
		}
        if (fabs(🐒🍌Stats.SmoothMoving.Y) >= 1.0f)
        {
            y += static_cast<int>(🐒🍌Stats.SmoothMoving.Y);
            🐒🍌Stats.SmoothMoving.Y -= static_cast<int>(🐒🍌Stats.SmoothMoving.Y);
        }
        SetCenter(x, y);
        if (IsOverlap(*this, *🍌)) {
            🍌->SetOwnerPos(static_cast<int>(GetCenter().X), static_cast<int>(GetCenter().Y));
			🍌->SetActive(false);
            🐒🍌Stats.Got🍌 = true;
		}
        return true;
    }
    bool Tower::No🍌😭() {
        🐒🍌Stats.Got🍌 = false;
        🐒🍌Stats.Target = nullptr;
        if (Vector2Distance(GetCenter(), 🐒🍌Stats.OriginPos) < 10.0f) {
            return false;
        }
        Vector2 VecMove = Normailize({ 🐒🍌Stats.OriginPos.X - GetCenter().X, 🐒🍌Stats.OriginPos.Y - GetCenter().Y });
        SetFrameIndexOfBitmap(min(GetFrameSizeOfBitmap() - 1, GetFrameIndexByVector2(VecMove)));
        VecMove.X = VecMove.X * 🐵Backspeed / Cavallo::Multiplier + GetCenter().X;
        VecMove.Y = VecMove.Y * 🐵Backspeed / Cavallo::Multiplier + GetCenter().Y;
        int x = static_cast<int>(VecMove.X);
        int y = static_cast<int>(VecMove.Y);
        🐒🍌Stats.SmoothMoving.X += VecMove.X - x;
        🐒🍌Stats.SmoothMoving.Y += VecMove.Y - y;
        if (fabs(🐒🍌Stats.SmoothMoving.X) >= 1.0f)
        {
            x += static_cast<int>(🐒🍌Stats.SmoothMoving.X);
            🐒🍌Stats.SmoothMoving.X -= static_cast<int>(🐒🍌Stats.SmoothMoving.X);
        }
        if (fabs(🐒🍌Stats.SmoothMoving.Y) >= 1.0f)
        {
            y += static_cast<int>(🐒🍌Stats.SmoothMoving.Y);
            🐒🍌Stats.SmoothMoving.Y -= static_cast<int>(🐒🍌Stats.SmoothMoving.Y);
        }
        SetCenter(x, y);
        return false;
    }
}
