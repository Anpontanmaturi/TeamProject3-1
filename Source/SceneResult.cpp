// SceneResult.cpp

#include "SceneResult.h"
#include "System/Graphics.h"
#include "System/Input.h"
#include "SceneTitle.h"
#include <imgui.h>
#include <SceneManager.h>
#include <SceneGame.h>
#include "ScoreManager.h"

void SceneResult::Initialize()
{
    // 背景画像
    sprite = new Sprite("Data/Sprite/Result.png");

    // BGM
    resultAu = Audio::Instance().LoadAudioSource("Data/Sound/tanoshiibouken.wav");
    resultAu->Play(true);

    // ランキングUI
    rankingUI = std::make_unique<UiRanking>();

    inputLockFrame = 0;
    canInput = false;

    //========================
    // ランク画像読み込み
    //========================

    rankS = std::make_unique<Sprite>(
        "Data/Sprite/S.png"
    );

    rankA = std::make_unique<Sprite>(
        "Data/Sprite/A.png"
    );

    rankB = std::make_unique<Sprite>(
        "Data/Sprite/B.png"
    );

    rankC = std::make_unique<Sprite>(
        "Data/Sprite/C.png"
    );
}

void SceneResult::Finalize()
{
    // ランキングUI
    if (rankingUI != nullptr)
    {
        rankingUI->Finalize();
    }

    rankingUI.reset();

    // 背景削除
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }

    // ランク画像解放
    rankS.reset();
    rankA.reset();
    rankB.reset();
    rankC.reset();

    // BGM停止
    resultAu->Stop();
}

void SceneResult::Update(float elapsedTime)
{
    // ランキングUI更新
    if (rankingUI)
    {
        rankingUI->Update(elapsedTime);
    }

    //====================
      // 3秒ロック（180F）
      //====================
    if (!canInput)
    {
        inputLockFrame++;

        if (inputLockFrame >= 180)
        {
            canInput = true;
        }
    }

    // SPACE入力
    if (canInput)
    {
        if (GetAsyncKeyState(VK_SPACE) & 0x8000)
        {
            SceneManager::Instance().ChangeScene(
                new SceneTitle()
            );
        }
    }

    // 入力
    GamePad& gamePad = Input::Instance().GetGamePad();

    
}

void SceneResult::Render()
{
    Graphics& graphics = Graphics::Instance();

    ID3D11DeviceContext* dc =
        graphics.GetDeviceContext();

    // 描画準備
    RenderContext rc;
    rc.deviceContext = dc;
    rc.renderState = graphics.GetRenderState();

    //========================
    // 背景描画
    //========================

    float screenWidth =
        static_cast<float>(graphics.GetScreenWidth());

    float screenHeight =
        static_cast<float>(graphics.GetScreenHeight());

    sprite->Render(
        rc,
        0,
        0,
        0,
        screenWidth,
        screenHeight,
        0,
        1,
        1,
        1,
        1
    );

    //========================
    // ランキングUI描画
    //========================

    if (rankingUI)
    {
        rankingUI->Render(rc);
    }

   


    //========================
    // スコア取得
    //========================

    int score =
        ScoreManager::Instance().GetLastPlayScore();

    //========================
    // ランク判定
    //========================

    if (score >= 10000)
    {
        currentRank = rankS.get();
    }
    else if (score >= 5000) 
    {
        currentRank = rankA.get();
    }
    else if (score >= 2500)
    {
        currentRank = rankB.get();
    }
    else
    {
        currentRank = rankC.get();
    }

    //========================
    // ランク描画
    //========================

    if (currentRank)
    {
        currentRank->Render(
            rc,

            855,   // X
            255,   // Y

            0,

            210,   // Width
            210,   // Height

            0,

            1,
            1,
            1,
            1
        );
    }
}

void SceneResult::DrawGUI()
{
    // GUIなし
}