#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <string>

#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

using namespace game_framework;

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame* g) : CGameState(g)
{
}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{
    Btd::Vector2 mapSize = {
        static_cast<float>(map.GetBackground().GetWidth()), static_cast<float>(map.GetBackground().GetHeight())
    };
    map.SetStartPosition({
        static_cast<float>(0), mapSize.Y * 0.4F
    });
    map.SetRoute({
        {mapSize.X * 0.11F, mapSize.Y * 0.4F}, {mapSize.X * 0.11F, mapSize.Y * 0.12F},
        {mapSize.X * 0.3F, mapSize.Y * 0.12F}, {mapSize.X * 0.3F, mapSize.Y * 0.67F},
        {mapSize.X * 0.05F, mapSize.Y * 0.67F}, {mapSize.X * 0.05F, mapSize.Y * 0.85F},
        {mapSize.X * 0.65F, mapSize.Y * 0.85F}, {mapSize.X * 0.65F, mapSize.Y * 0.53F},
        {mapSize.X * 0.45F, mapSize.Y * 0.53F}, {mapSize.X * 0.45F, mapSize.Y * 0.3F},
        {mapSize.X * 0.65F, mapSize.Y * 0.3F}, {mapSize.X * 0.65F, mapSize.Y * 0.06F},
        {mapSize.X * 0.38F, mapSize.Y * 0.06F}, {mapSize.X * 0.38F, mapSize.Y * 0.F},
        {mapSize.X * 0.38F, mapSize.Y * -0.08F},
    });
}

void CGameStateRun::OnMove() // 移動遊戲元素
{
    UnitTest.UnitTest();
}

vector<string> GeneratePath(string initPath)
{
    vector<string> path;
    for (int i = 1; i <= 8; i++)
    {
        string tmpPath = initPath + std::to_string(i) + ".bmp";
        path.push_back(tmpPath);
    }
    return path;
}

void CGameStateRun::OnInit() // 遊戲的初值及圖形設定
{
    monkey.LoadBitmapByString({{GeneratePath("resources/towers/monkey/tower_monkey_")}}, RGB(0, 0, 0));
    map.InitRoad();
    map.InitBackground();
    // unit init
    UnitTest = Btd::TestEverything();
    UnitTest.UnitInit();
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == VK_F1)
    {
        int index = (monkey.GetFrameIndexOfBitmap() + 1) % 8;
        monkey.SetFrameIndexOfBitmap(index);
    }
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point) // 處理滑鼠的動作
{
    if (monkey.IsMovable())
    {
        monkey.SetNotMove();
    }
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point) // 處理滑鼠的動作
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point) // 處理滑鼠的動作
{
    if (monkey.IsMovable())
    {
        POINT p;
        GetCursorPos(&p);
        HWND hwnd = FindWindowA(nullptr, "Game");
        ScreenToClient(hwnd, &p);
        monkey.SetCenter(p.x, p.y);
    }
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point) // 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point) // 處理滑鼠的動作
{
}

void CGameStateRun::OnShow()
{
    map.ShowBackground();
    map.ShowRoad();
    monkey.ShowBitmap();
    UnitTest.UnitShow();
}
