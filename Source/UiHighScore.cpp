#include "UiHighScore.h"
#include "SceneGame.h"
#include "ScoreManager.h"
#include "System/Graphics.h"

UiHighScore::UiHighScore()
{
	// テクスチャの読み込み
	highScore = std::make_unique<Sprite>("Data/Sprite/UI/topScore.png");

	// SpriteBatchとSpriteFontの初期化(フォント用)
	auto& graphics = Graphics::Instance();
	auto device = graphics.GetDevice();
	auto context = graphics.GetDeviceContext();
	spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Data/Font/Font1.spritefont");

	// 初期位置とサイズ
	SetPosition(5.0f, 90.0f);
	SetScale(0.5f, 0.5f);
}

UiHighScore::~UiHighScore()
{
	spriteFont.reset();
	spriteBatch.reset();
}

void UiHighScore::Update(float elapsedTime)
{
	// ハイスコアの値を取得
	float highScore = ScoreManager::Instance().GetHighScore();
	// ハイスコアの値をテキストに変換
	text = std::to_wstring(static_cast<int>(highScore));
	// フォントの拡大率をUIのスケールに合わせる
	fontScale = { scale.x * scaleMag, scale.y * scaleMag };
	fontScale.x *= 0.8f; // スコアのフォントの調整
	// フォントの描画位置を計算
	DirectX::XMVECTOR sizeVec = spriteFont->MeasureString(text.c_str());
	float baseWidth = DirectX::XMVectorGetX(sizeVec);
	// 実際の描画幅を計算
	float actualWidth = baseWidth * fontScale.x;
	fontPos.x = position.x + (FontOffset.x * scale.x) - actualWidth / 2.0f;
	fontPos.y = position.y + (FontOffset.y * scale.y);
}

void UiHighScore::Render(const RenderContext& rc)
{
	// 親の描画
	highScore->Render(rc, position.x, position.y, 0.1f,
		Origin_W * scale.x, Origin_H * scale.y, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f);
	// 文字の描画
	spriteBatch->Begin();
	spriteFont->DrawString(
		spriteBatch.get(),
		text.c_str(),
		fontPos,
		fontColor, 0.0f, { 0.0f, 0.0f }, fontScale,
		DirectX::SpriteEffects_None, 0.0f);
	spriteBatch->End();
}