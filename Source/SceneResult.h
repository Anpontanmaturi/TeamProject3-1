
// SceneResult.h
#pragma once
#include "Scene.h"
#include "System/Sprite.h"
#include "UiRanking.h"
#include "System/Audio.h"
#include "System/AudioSource.h"

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
    AudioSource* resultAu = nullptr;
    AudioSource* resultSe = nullptr;
    std::unique_ptr<Sprite> rankS;
    std::unique_ptr<Sprite> rankA;
    std::unique_ptr<Sprite> rankB;
    std::unique_ptr<Sprite> rankC;

    Sprite* currentRank = nullptr;
};
