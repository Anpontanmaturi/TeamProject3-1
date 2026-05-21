#include "System/Graphics.h"
#include "SceneTitle.h"
#include "System/Input.h"
#include "SceneGame.h"
#include "SceneLoading.h"
#include "SceneManager.h"
#include "Sceneplay.h"
#include <imgui.h>
#include <System/Audio.h>
#include <cmath>

// 初期化
void SceneTitle::Initialize()
{
	// スプライト初期化
	spriteBG = new Sprite("Data/Sprite/tai.png");
	spriteStart = new Sprite("Data/Sprite/start.png");
	spriteTutorial = new Sprite("Data/Sprite/tutorial.png");
	spriteSentaku = new Sprite("Data/Sprite/yazirusi.png");

	// BGM
	ST = Audio::Instance().LoadAudioSource("Data/Sound/tai.wav");
	// SE
	selectSE = Audio::Instance().LoadAudioSource("Data/Sound/select.wav");
	enterSE = Audio::Instance().LoadAudioSource("Data/Sound/enter.wav");

	choice = true;
	timer = 0;

	// ループ再生
	ST->Play(true);
	// 初回再生準備
	selectSE->Stop();
	selectSE->Play(false);

	enterSE->Play(false);
	enterSE->Stop();
}

// 終了処理
void SceneTitle::Finalize()
{
	delete spriteBG;
	spriteBG = nullptr;

	delete spriteStart;
	spriteStart = nullptr;

	delete spriteTutorial;
	spriteTutorial = nullptr;

	delete spriteSentaku;
	spriteSentaku = nullptr;

	delete ST;
	ST = nullptr;

	delete selectSE;
	selectSE = nullptr;

	delete enterSE;
	enterSE = nullptr;
}

// 更新処理
void SceneTitle::Update(float elapsedTime)
{
	timer++;

	static bool prevUp = false;
	static bool prevDown = false;
	static bool prevSpace = false;

	bool nowUp = (GetAsyncKeyState('W') & 0x8000) != 0;
	bool nowDown = (GetAsyncKeyState('S') & 0x8000) != 0;
	bool nowSpace = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;

	// 上入力
	if (nowUp && !prevUp)
	{
		if (!choice)
		{
			choice = true;

			selectSE->Stop();
			selectSE->Play(false);
		}
	}

	// 下入力
	if (nowDown && !prevDown)
	{
		if (choice)
		{
			choice = false;

			selectSE->Stop();
			selectSE->Play(false);
		}
	}

	// 決定
	if (nowSpace && !prevSpace)
	{
		enterSE->Stop();
		enterSE->Play(false);

		ST->Stop();

		changingScene = true;
		sceneChangeTimer = 0.0f;
	}

	prevUp = nowUp;
	prevDown = nowDown;
	prevSpace = nowSpace;
	// シーン切り替え待機
	if (changingScene)
	{
		sceneChangeTimer += elapsedTime;

		if (sceneChangeTimer >= 0.6f)
		{
			if (choice)
			{
				SceneManager::Instance().ChangeScene(
					new SceneLoading(new SceneGame)
				);
			}
			else
			{
				SceneManager::Instance().ChangeScene(
					new SceneLoading(new ScenePlay)
				);
			}
		}
	}
}

// 描画処理
void SceneTitle::Render()
{
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
	

	float width = 175.0f;
	float height = 75.0f;

	// START
	spriteStart->Render(
		rc,
		610, 525, 0,
		width,
		height,
		0,
		1, 1, 1, 1
	);

	// TUTORIAL
	spriteTutorial->Render(
		rc,
		610, 625, 0,
		width,
		height,
		0,
		1, 1, 1, 1
	);
	// 矢印アニメーション
	float arrowY = choice ? 525.0f : 625.0f;

	// 左右にふわふわ動く
	float arrowX = 520.0f + sinf(timer * 0.1f) * 10.0f;

	spriteSentaku->Render(
		rc,
		arrowX, arrowY, 0,
		70, 70,
		0,
		1, 1, 1, 1
	);
}

// GUI描画
void SceneTitle::DrawGUI()
{

}