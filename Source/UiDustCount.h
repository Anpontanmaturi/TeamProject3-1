#pragma once
#include "UiBase.h"
#include "System/Sprite.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <string>
#include <memory>

class UIDustCount : public UIBase
{
public:
	UIDustCount();
	virtual ~UIDustCount() override;

	// 更新
	virtual void Update(float elapsedTime) override;
	// 描画
	virtual void Render(const RenderContext& rc) override;

	// 加算されるスコア
	void SetAddedScore(int amount) { ScoreText = L"+" + std::to_wstring(amount); }

	// セッター・ゲッター
	DirectX::XMFLOAT2 GetFontOffset() const { return FontOffset; }
	void SetFontOffset(float x, float y) { FontOffset = { x, y }; }
	DirectX::XMFLOAT2 GetAddedScoreOffset() const { return AddedScoreOffset; }
	void SetAddedScoreOffset(float x, float y) { AddedScoreOffset = { x, y }; }

private:
	

	// テクスチャ
	std::unique_ptr<Sprite> counter;

	// --- フォント描画用 ---
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont>  spriteFont;

	DirectX::XMFLOAT2 FontOffset = { 220.0f, 110.0f };
	DirectX::XMFLOAT2 fontPos{};
	DirectX::XMFLOAT2 fontScale{};
	DirectX::XMVECTORF32 fontColor = DirectX::Colors::White;
	std::wstring text; // 描画するテキスト
	int lastCount = -1; // 前回のカウント値（初期値は-1）

	// 加算スコアを表示するためのオフセット
	DirectX::XMFLOAT2 AddedScoreOffset = { 240.0f, 110.0f };
	DirectX::XMFLOAT2 AddedScorePos{};
	DirectX::XMFLOAT2 AddedScoreFontScale{};
	DirectX::XMVECTORF32 AddedScoreFontColor = DirectX::Colors::White;
	std::wstring ScoreText = L"+" + std::to_wstring(0); // 加算スコアのテキスト
	int addedScoreLastValue = -1; // 前回の加算スコア値（初期値は-1）

	// 画像の元のサイズ
	const float Origin_W = 500.0f;
	const float Origin_H = 200.0f;

	float scaleMag = 2.5f; // フォントの拡大率（UIのスケールに合わせて調整）
};