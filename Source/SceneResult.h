
// SceneResult.h
#pragma once
#include "Scene.h"
#include "System/Sprite.h"
#include "UiRanking.h"

class SceneResult : public Scene
{
public:
    void Initialize() override;
    void Finalize() override;
    void Update(float elapsedTime) override;
    void Render() override;
    void DrawGUI() override;


private:
    Sprite* sprite = nullptr;

    std::unique_ptr<UiRanking> rankingUI;
};
