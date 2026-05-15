#include "UiTimer.h"
#include "SceneGame.h"
#include "System/Graphics.h"

UiTimer::UiTimer()
{
	// テクスチャの読み込み
	timer = std::make_unique<Sprite>("Data/Sprite/UI/clock.png");

	// SpriteBatchとSpriteFontの初期化(フォント用)
	auto& graphics = Graphics::Instance();
	auto device = graphics.GetDevice();
	auto context = graphics.GetDeviceContext();

	spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Data/Font/Font1.spritefont");

	// 初期位置とサイズ
	SetPosition(0.0f,0.0f);
	SetScale(0.5f, 0.5f);
}

UiTimer::~UiTimer()
{
}

void UiTimer::Update(float elapsedTime)
{
	if (!spriteFont)_ASSERT_EXPR(false, L"SpriteFontが初期化されていません");

	// タイマーの残り時間を取得
	float timerValue = SceneGame::Instance().GetTimer();

	// タイマーの値を分と秒でテキストに変換
	int minutes = static_cast<int>(timerValue) / 60;
	int seconds = static_cast<int>(timerValue) % 60;
	text = std::to_wstring(minutes) + L":" + (seconds < 10 ? L"0" : L"") + std::to_wstring(seconds);

	// フォントの拡大率をUIのスケールに合わせる
	fontScale = { scale.x * scaleMag, scale.y * scaleMag };

	fontPos.x = position.x + (FontOffset.x * scale.x);
	fontPos.y = position.y + (FontOffset.y * scale.y);
}

void UiTimer::Render(const RenderContext& rc)
{
	// 描画
	float dx = position.x;
	float dy = position.y;
	timer->Render(rc, dx, dy, 0.1f,
		Origin_W * scale.x, Origin_H * scale.y, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f);

	// 文字の描画
	spriteBatch->Begin();

	spriteFont->DrawString(
		spriteBatch.get(),
		text.c_str(),
		fontPos,
		fontColor,
		0.0f,                   // 回転
		DirectX::XMFLOAT2(0, 0), // 原点
		fontScale       // スケールを画像に合わせる
	);

	spriteBatch->End();
}
