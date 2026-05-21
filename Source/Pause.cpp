#include "Pause.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "System/Graphics.h"
#include <Windows.h>
#include "SceneLoading.h"
#include "SceneGame.h"
void Pause::Initialize()
{
    spriteBG = new Sprite("Data/Sprite/menu.png");
    spriteTai = new Sprite("Data/Sprite/titlereturn.png");
    spriteModo = new Sprite("Data/Sprite/return.png");
    spriteSentaku = new Sprite("Data/Sprite/yazirusi.png");

    isPaused = false;
    keyPrev = false;
    timer = 0;
}
void Pause::Finalize()
{
    delete spriteBG;
    spriteBG = nullptr;

    delete spriteTai;
    spriteTai = nullptr;

    delete spriteModo;
    spriteModo = nullptr;

    delete spriteSentaku;
    spriteSentaku = nullptr;
   
}
void Pause::Update()
{
    timer++;
    // Pキーでポーズ切替
    bool now = (GetAsyncKeyState('P') & 0x8000) != 0;

    static bool prevUp = false;
    static bool prevDown = false;
    static bool prevSpace = false;

    bool nowUp = (GetAsyncKeyState('W') & 0x8000) != 0;
    bool nowDown = (GetAsyncKeyState('S') & 0x8000) != 0;
    bool nowSpace = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;

    if (now && !keyPrev)
    {
        isPaused = !isPaused;
    }
    keyPrev = now;

    // ★ ポーズ中だけ操作可能
    if (!isPaused) return;

    // 上入力
    if (nowUp && !prevUp)
    {
        choice = true;
    }

    // 下入力
    if (nowDown && !prevDown)
    {
        choice = false;
    }

    // 決定
    if (nowSpace && !prevSpace)
    {


        if (choice)
        {
            SceneManager::Instance().ChangeScene(
                new SceneLoading(new SceneTitle)
            );
        }
        else
        {
            isPaused = false;
        }
    }

    prevUp = nowUp;
    prevDown = nowDown;
    prevSpace = nowSpace;
}

void Pause::Render()
{
    if (!isPaused) return;

    Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();

	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = graphics.GetRenderState();

	float screenWidth = (float)graphics.GetScreenWidth();
	float screenHeight = (float)graphics.GetScreenHeight();

	// 背景
	spriteBG->Render(
		rc,
		0, 0, 0,
		screenWidth, screenHeight,
		0,
		1, 1, 1, 1
	);

	// 選択中は拡大
	

	float width = 475.0f;
	float height = 200.0f;

	// START
	spriteTai->Render(
		rc,
		510, 125, 0,
		width,
		height,
		0,
		1, 1, 1, 1
	);

	// TUTORIAL
	spriteModo->Render(
		rc,
		510, 375, 0,
		width,
		height,
		0,
		1, 1, 1, 1
	);
	

    // 矢印サイズ
    float arrowSize = 90.0f;

    // 左側に配置
    float arrowBaseX = 420.0f;

    // 選択項目の中央Y
    float arrowY = choice ? 200.0f : 475.0f;

    // 左右にふわふわ
    float arrowX = arrowBaseX + sinf(timer * 0.1f) * 10.0f;

    spriteSentaku->Render(
        rc,
        arrowX,
        arrowY,
        0,
        arrowSize,
        arrowSize,
        0,
        1, 1, 1, 1
    );
}