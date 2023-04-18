#include "stdafx.h"
#include "TestEverything.h"

#include "TowerFactory.h"

namespace Btd
{
    void TestEverything::UnitInit()
    {
        vector<string> ThrowablePath = {
            "resources/towers/monkey/tower_monkey_1.bmp", "resources/towers/monkey/tower_monkey_2.bmp",
            "resources/towers/monkey/tower_monkey_3.bmp", "resources/towers/monkey/tower_monkey_4.bmp",
            "resources/towers/monkey/tower_monkey_5.bmp", "resources/towers/monkey/tower_monkey_6.bmp",
            "resources/towers/monkey/tower_monkey_7.bmp", "resources/towers/monkey/tower_monkey_8.bmp"
        };
        switch (_unitTestState)
        {
        case Bez:
            bezObject.LoadBitmapByString(ThrowablePath,RGB(255, 255, 255));
            bezObject.SetAnimation(20, false);
            bezObject.SetTopLeft(0, 0);
            BezCounter = 0;
            BezDelta = 0.05F;

        case BoomerangTest:
            _boomerang.LoadBitmapByString(ThrowablePath,RGB(255, 255, 255));
            _boomerang.SetAnimation(20, false);
            _boomerang.SetTopLeft(100, 100);
            _boomerang.SetRoute({
                {static_cast<float>(0), static_cast<float>(250)},
                {static_cast<float>(500), static_cast<float>(0)},
                {static_cast<float>(500), static_cast<float>(500)},
                {static_cast<float>(0), static_cast<float>(250)}
            });

        case BoomerangMonkeyTest:
            bm = make_shared<BoomerangMonkey>(BoomerangMonkey());
            bm->LoadBitmapByString({
                                       "resources/towers/monkey/tower_monkey_1.bmp",
                                       "resources/towers/monkey/tower_monkey_2.bmp",
                                       "resources/towers/monkey/tower_monkey_3.bmp",
                                       "resources/towers/monkey/tower_monkey_4.bmp",
                                       "resources/towers/monkey/tower_monkey_5.bmp",
                                       "resources/towers/monkey/tower_monkey_6.bmp",
                                       "resources/towers/monkey/tower_monkey_7.bmp",
                                       "resources/towers/monkey/tower_monkey_8.bmp"
                                   }, RGB(0, 0, 0));
            bm->SetCenter(300, 300);
            bm->SetIsMove(false);
            bm->SetShootDeltaTime(3);
            bm->SetActive(true);
            bm->SetFrameIndexOfBitmap(6);
            bm->RangeCircle.LoadBitmapByString({"resources/towers/range.bmp", "resources/towers/range_red.bmp"},
                                               RGB(0, 0, 0));
            bm->RangeCircle.SetCenter(GetCursorPosX(), GetCursorPosY());
        default:
            break;
        }
    }

    void TestEverything::UnitTest()
    {
        switch (_unitTestState)
        {
        case Bez:

            auto position = BezierCurve4Point(
                {static_cast<float>(0), static_cast<float>(250)},
                {static_cast<float>(500), static_cast<float>(0)},
                {static_cast<float>(500), static_cast<float>(500)},
                {static_cast<float>(0), static_cast<float>(250)}, BezCounter);
            if (BezCounter >= 1)
            {
                BezCounter = 0;
                _unitTestState = BoomerangTest;
                // BezDelta = -0.01F;
            }
            if (BezCounter <= 0)
            {
                BezDelta = 0.05F;
            }
            BezCounter = BezCounter + BezDelta;
            bezObject.SetTopLeft(static_cast<int>(position.X), static_cast<int>(position.Y));
            break;
        case BoomerangTest:
            _boomerang.Update();
            if (_boomerang.GetCenter().X > 100)
            {
                _unitTestState = BoomerangMonkeyTest;
            }
            break;
        case BoomerangMonkeyTest:
            bm->Update();
            break;
        default: ;
        }
    }

    void TestEverything::UnitShow()
    {
        switch (_unitTestState)
        {
        case Bez:
            bezObject.ShowBitmap();
            break;
        case BoomerangTest:
            _boomerang.ShowBitmap();
            break;
        case BoomerangMonkeyTest:
            bm->TowerShow();
            break;

        default:
            break;
        }
    }
}
