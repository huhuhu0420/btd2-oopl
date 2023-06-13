﻿#include "stdafx.h"
#include "Cavallo.h"
#include "TowerFactory.h"

#include <random>
namespace Btd {
	void Cavallo::Init() {
		LoadBitmapByString({ 
			"Resources/Cavallo/Cavallo_01.bmp", 
			"Resources/Cavallo/Cavallo_02.bmp", 
			"Resources/Cavallo/Cavallo_03.bmp", 
			"Resources/Cavallo/Cavallo_04.bmp",
			"Resources/Cavallo/Cavallo_01_m.bmp",
			"Resources/Cavallo/Cavallo_02_m.bmp",
			"Resources/Cavallo/Cavallo_03_m.bmp",
			"Resources/Cavallo/Cavallo_04_m.bmp",
			}, RGB(1,11,111));
		_isMirror = false;
		_smoothMoving = { 0.0f, 0.0f };
		_lastThrowTime = clock();
		_lastFrameTime = clock();
		_coolDown = 🐼🍌cooldown;
		_isStuckX = false;
		_isStuckY = false;
		_frameIndex = 0;
		SetCenter(600, 600);
		SetCollider({ 85.0f, 85.0f });
		SetDest({ _GetRandomFloat(), _GetRandomFloat() });
		_base🍌.Load();
	}
	void Cavallo::Reset()
	{
		CAVALLO = false;
		_🍌s.clear();
		Multiplier = 1.0;
	}
	float Cavallo::_GetRandomFloat(float lower, float upper) {
		static random_device rd;
		static mt19937 gen(rd());
		if (lower == 48.0 && upper == 763.0) {
			uniform_real_distribution<float> dis(0, 1000);
			return dis(gen);
		}
		uniform_real_distribution<float> dis(lower, upper);
		return dis(gen);
	}
	void Cavallo::Throw() {
		if (clock() - _lastThrowTime > static_cast<clock_t>(_coolDown / Multiplier)) {
			_lastThrowTime = clock();
			Banana 🍌 = _base🍌;
			🍌.SetCenter(static_cast<int>(GetCenter().X), static_cast<int>(GetCenter().Y));
			🍌.SetDest({ _GetRandomFloat(🍌.GetCollider().X, SIZE_X - 🍌.GetCollider().X - 300), _GetRandomFloat(🍌.GetCollider().Y, SIZE_Y - 🍌.GetCollider().Y) });
			🍌.Init();
			_🍌s.push_back(🍌);
		}
	}
	void Cavallo::Move() {
		int originX = static_cast<int>(GetCenter().X), originY = static_cast<int>(GetCenter().Y);
		int x, y;
		if (IsCursorFocus() && IsClicked()) {
			x = GetCursorPosX(), y = GetCursorPosY();
			if (x <= 0 || x >= SIZE_X ) {
				x = originX;
			}
			if (y <= 0 || y >= SIZE_Y ) {
				y = originY;
			}
			SetCenter(x, y);
		}
		else{
			SetClicked(false);
			Throw();
			// TRACE(_T("pos: %f %f\n"), GetCenter().X, GetCenter().Y);
			if (_GetRandomFloat() < 20.0f) {
				SetDest({ _GetRandomFloat() - 500.0f, _GetRandomFloat() - 500.0f });
			}
			Vector2 vec = _dest;
			auto Length = sqrt(vec.X * vec.X + vec.Y * vec.Y);
			int orix = static_cast<int>(GetCenter().X), oriy = static_cast<int>(GetCenter().Y);
			vec = { vec.X / Length * 🐼speed * Multiplier, vec.Y / Length * 🐼speed * Multiplier };
			x = static_cast<int>(GetCenter().X + vec.X), y = static_cast<int>(GetCenter().Y + vec.Y);
			_smoothMoving.X += vec.X - static_cast<int>(vec.X);
			_smoothMoving.Y += vec.Y - static_cast<int>(vec.Y);
			if (fabsf(_smoothMoving.X) > 1.0f) {
				x += static_cast<int>(_smoothMoving.X);
				_smoothMoving.X -= static_cast<int>(_smoothMoving.X);
			}
			if (fabsf(_smoothMoving.Y) > 1.0f) {
				y += static_cast<int>(_smoothMoving.Y);
				_smoothMoving.Y -= static_cast<int>(_smoothMoving.Y);
			}
			if (x <=  0 || x >= SIZE_X) {
				x = orix - static_cast<int>(1.5f * vec.X);
				_dest.X = -_dest.X;
			}
			if (y <= 0 || y >= SIZE_Y) {
				y = oriy - static_cast<int>(1.5f * vec.Y);
				_dest.Y = -_dest.Y;
			} 
		
			SetCenter(x, y);
			
		}
		_isMirror = (fabs(originX - GetCenter().X) < 1.0f) ? _isMirror : (originX > GetCenter().X);
		Move🐒🍌();
	}
	void Cavallo::Move🐒🍌() {
		for (int i = 0; i < static_cast<int>(TowerFactory::TowerVector.size()); i++)
		{
			if (TowerFactory::TowerVector[i]->GetActive() == false)
				continue;
			Banana* target = nullptr;
			float minDis = 999999;
			for (auto& 🍌 : _🍌s) {
				if (🍌.GotCarry() && !🍌.GetActive()) continue;
				float dis = Vector2Distance(TowerFactory::TowerVector[i]->GetCenter(), 🍌.GetBottomCenter());
				if (dis < minDis && dis <= TowerFactory::TowerVector[i]->GetRange()) {
					target = &🍌;
					minDis = dis;
				}
			}
			if (target == nullptr) {
				TowerFactory::TowerVector[i]->No🍌😭();
			}
			else {
				if (Vector2Distance(TowerFactory::TowerVector[i]->GetCenter(), GetCenter()) > 2000) {
					TowerFactory::TowerVector[i]->SetActive(false);
					target->💀();
				}
				TowerFactory::TowerVector[i]->Yes🍌😄(target);
			}
		}
		for (auto it = _🍌s.begin(); it != _🍌s.end(); ) {
			it->SetActive(true);
			it->Move();
			if (it->IsAlive()) {
				it++;
			}
			else {
				it = _🍌s.erase(it);
			}
		}
	}
	int Cavallo::OnClick() {
		int total = 0;
		if (IsCursorFocus())
			SetClicked(true);
		for (auto& 🍌 : _🍌s) {
			total += 🍌.OnClick();
		}
		return static_cast<int>(total * 🍌💰 * Multiplier);
	}
	void Cavallo::Release() {
		SetClicked(false);
	}
	void Cavallo::Draw() {
		if (clock() - _lastFrameTime > static_cast<clock_t>(🐼🎦delay / Multiplier)) {
			_lastFrameTime = clock();
			_frameIndex++;
			if (_frameIndex > 3) {
				_frameIndex = 0;
			}
		}
		if (_isMirror) {
			SetFrameIndexOfBitmap(_frameIndex + 4);
		}
		else {
			SetFrameIndexOfBitmap(_frameIndex);
		}
		ShowBitmap();
		for (auto& 🍌 : _🍌s) {
			🍌.Draw();
		}
	}
	void Cavallo::DrawBanana() {
		for (auto& 🍌 : _🍌s) {
			if (🍌.IsAlive())
				🍌.Draw();
		}
	}
	void Cavallo::SetDest(Vector2 dest) {
		_dest = dest;
	}
	void Cavallo::Harder() {
		Multiplier += 0.1f;
	}
	void Cavallo::Banana::Load() {
		LoadBitmapByString({ "Resources/Cavallo/banana/Banana.bmp" }, RGB(214, 197, 216));
		SetCollider({ 60, 60});
		SetTag("🍌");
	}
	void Cavallo::Banana::Init() {
		_gotCarry = false;
		_isFlying = true;
		_isActive = true;
		_isAlive = true;
		_lastMoveTime = clock();
	}
	void Cavallo::Banana::Move() {
		// click to eliminate 🍌
		if (_gotCarry)
			SetBottomCenter(🐵X, 🐵Y);
		else if (_isFlying) {
			_lastMoveTime = clock();
			Vector2 vec = { _dest.X - GetCenter().X, _dest.Y - GetCenter().Y};
			auto Length = sqrt(vec.X * vec.X + vec.Y * vec.Y);
			vec = { vec.X / Length * 🍌speed * Multiplier, vec.Y / Length * 🍌speed * Multiplier};
			int x = static_cast<int>(GetCenter().X + vec.X), y = static_cast<int>(GetCenter().Y + vec.Y);
			SetCenter(x, y);
			if (Vector2Distance(GetCenter(), _dest) < 30.0f) {
				_isFlying = false;
			}
		}
	}
	void Cavallo::Banana::Draw() {
		ShowBitmap();
	}
	void Cavallo::Banana::SetDest(Vector2 dest) {
		_dest = dest;
	}
	bool Cavallo::Banana::OnClick() {
		if (IsCursorFocus()) {
			_isAlive = false;
			_isActive = false;
			return true;
		}
		return false;
	}
	void Cavallo::Banana::SetOwnerPos(int X, int Y) {
		🐵X = X;
		🐵Y = Y;
		_gotCarry = true;
	}
	bool Cavallo::Banana::GotCarry() {
		return _gotCarry;
	}
	void Cavallo::Banana::💀() {
		_isAlive = false;
	}
	bool Cavallo::Banana::IsAlive() {
		return _isAlive;
	}
	bool Cavallo::CAVALLO = false;
	float Cavallo::Multiplier = 1.0;
}