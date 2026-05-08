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
    bool keyPrevT = false;

    Sprite* sprite = nullptr;
};