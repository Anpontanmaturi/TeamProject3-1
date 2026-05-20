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
    // ”wŒi‰æ‘œ
    sprite = new Sprite("Data/Sprite/Result.png");

    // BGM
    resultAu = Audio::Instance().LoadAudioSource("Data/Sound/tanoshiibouken.wav");
    resultAu->Play(true);

    // ƒ‰ƒ“ƒLƒ“ƒOUI
    rankingUI = std::make_unique<UiRanking>();

    //========================
    // ƒ‰ƒ“ƒN‰æ‘œ“Ç‚İ‚İ
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
    // ƒ‰ƒ“ƒLƒ“ƒOUI
    if (rankingUI != nullptr)
    {
        rankingUI->Finalize();
    }

    rankingUI.reset();

    // ”wŒiíœ
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }

    // ƒ‰ƒ“ƒN‰æ‘œ‰ğ•ú
    rankS.reset();
    rankA.reset();
    rankB.reset();
    rankC.reset();

    // BGM’â~
    resultAu->Stop();
}

void SceneResult::Update(float elapsedTime)
{
    // ƒ‰ƒ“ƒLƒ“ƒOUIXV
    if (rankingUI)
    {
        rankingUI->Update(elapsedTime);
    }

    // “ü—Í
    GamePad& gamePad = Input::Instance().GetGamePad();

    if ((GetAsyncKeyState(VK_SPACE) & 0x8000))
    {
        SceneManager::Instance().ChangeScene(
            new SceneTitle()
        );
    }
}

void SceneResult::Render()
{
    Graphics& graphics = Graphics::Instance();

    ID3D11DeviceContext* dc =
        graphics.GetDeviceContext();

    // •`‰æ€”õ
    RenderContext rc;
    rc.deviceContext = dc;
    rc.renderState = graphics.GetRenderState();

    //========================
    // ”wŒi•`‰æ
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
    // ƒ‰ƒ“ƒLƒ“ƒOUI•`‰æ
    //========================

    if (rankingUI)
    {
        rankingUI->Render(rc);
    }

    //========================
    // ƒXƒRƒAæ“¾
    //========================

    int score =
        ScoreManager::Instance().GetLastPlayScore();

    //========================
    // ƒ‰ƒ“ƒN”»’è
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
    // ƒ‰ƒ“ƒN•`‰æ
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
    // GUI‚È‚µ
}