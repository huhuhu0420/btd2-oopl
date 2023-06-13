#include "stdafx.h"
#include "SoundManager.h"

#include <array>

#include "../../Library/audio.h"

namespace Btd
{
    
    bool SoundManager::mute = false;
    vector<string> SoundManager::soundName ;
    int SoundManager::index = 0;
    vector<soundCounter> SoundManager::soundTimePool = vector<soundCounter>();
    array<bool,4> SoundManager::soundUsed = {false,false,false,false};

    void SoundManager::Init()
    {
        soundName.push_back("background.mp3");
        soundName.push_back("pop.mp3");
        soundName.push_back("lead.mp3");
        soundName.push_back("bomb.mp3");
    }

    void SoundManager::musicStop(BtdSound type)
    {
        game_framework::CAudio *audio = game_framework::CAudio::Instance();
        audio->Stop((int)type);
    }

    void SoundManager::musicPlay(BtdSound type, bool loop)
    {
        if(!mute)
        {
            if(soundUsed[(int)type])
            {
                return;
            }
            soundCounter next ={-1,type,-1,false};
            for(auto&s:soundTimePool )
            {
                if(s.type==type&&s.counter<=0)
                {
                    next = s;
                    s.counter=soundTime;
                    break;
                }
            }
            if(next.index==-1)
            {
                next = makeNewSound(type,loop);
            }
            game_framework::CAudio *audio = game_framework::CAudio::Instance();
            audio->Play(next.index, next.loop);
            soundUsed[(int)type]=true;
        }
    }

    void SoundManager::musicResume()
    {
        game_framework::CAudio *audio = game_framework::CAudio::Instance();
        audio->Resume();
    }

    void SoundManager::Update()
    {
        for(auto &s:soundUsed)
        {
            s=false;
        }
        
        for(auto&s : soundTimePool)
        {
            if(s.counter>0)
            {
                s.counter-=1;
            }
        }
    }

    soundCounter SoundManager::makeNewSound(BtdSound type, bool loop)
    {
        soundCounter s = {soundTime,type,index,loop};
        game_framework::CAudio *audio = game_framework::CAudio::Instance();
        string path = "Resources/sound/";
        string local = path+soundName[(int)type];
        audio->Load(index,const_cast<char*>(local.c_str())); // Probably bad API design
        soundTimePool.push_back(s);
        index++;
        return s;
    }
}
