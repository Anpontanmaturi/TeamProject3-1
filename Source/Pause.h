#pragma once
#include "System/Sprite.h"

class Pause
{
public:
    void Initialize();
    void Update();
    void Render();
    void Finalize();

    bool IsPaused() const { return isPaused; }

private:
    bool isPaused = false;
    bool keyPrev = false;
  

    bool choice = true; // true = Start, false = Tutorial

    Sprite* spriteBG = nullptr;
    Sprite* spriteTai = nullptr;
    Sprite* spriteModo = nullptr;
    Sprite* spriteSentaku = nullptr;
    int timer = 0;
};