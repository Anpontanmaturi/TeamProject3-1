#include "Pause.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "System/Graphics.h"
#include <Windows.h>
#include "SceneLoading.h"
#include "SceneGame.h"
void Pause::Initialize()
{
    sprite = new Sprite("Data/Sprite/ポーズ画面.png");

    isPaused = false;
    keyPrev = false;
}
void Pause::Finalize()
{
    delete sprite;
    sprite = nullptr;
}
void Pause::Update()
{
    // Pキーでポーズ切替
    bool now = (GetAsyncKeyState('P') & 0x8000) != 0;

    if (now && !keyPrev)
    {
        isPaused = !isPaused;
    }
    keyPrev = now;

    // ★ ポーズ中だけ操作可能
    if (!isPaused) return;

    static bool prevT = false;
    static bool prevR = false;

    bool nowT = (GetAsyncKeyState('T') & 0x8000) != 0;
    bool nowR = (GetAsyncKeyState('R') & 0x8000) != 0;

    // タイトルへ
    if (nowT && !prevT)
    {
        SceneManager::Instance().ChangeScene(
            new SceneLoading(new SceneTitle)
        );
    }

    // 再開
    if (nowR && !prevR)
    {
        isPaused = false;
    }

    prevT = nowT;
    prevR = nowR;
}

void Pause::Render()
{
    if (!isPaused || !sprite) return;

    Graphics& graphics = Graphics::Instance();

    RenderContext rc;
    rc.deviceContext = graphics.GetDeviceContext();
    rc.renderState = graphics.GetRenderState();

    float w = graphics.GetScreenWidth();
    float h = graphics.GetScreenHeight();

    // 画面全体に表示
    sprite->Render(
        rc,
        0, 0,
        0.0f,
        w, h,
        0.0f,
        1, 1, 1, 1
    );
}