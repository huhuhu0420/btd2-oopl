﻿#include "stdafx.h"
#include "FactoryButton.h"
#include "TowerFactory.h"

namespace Btd
{
    vector<shared_ptr<Tower>> TowerFactory::TowerVector = {};
    void FactoryButton::SetAttribute(TowerType attribute)
    {
        _attribute = attribute;
    }
    void FactoryButton::Update()
    {
        if (_isClicked == true)
        {
            TowerFactory::MakeTower(_attribute);
            SetClicked(false);
        }
    }

    
}
