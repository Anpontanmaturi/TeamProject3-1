#pragma once
#include "UiBase.h"
#include "System/Sprite.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <string>
#include <memory>

class UiMyScore : public UIBase
{
public:
	UiMyScore();
	virtual ~UiMyScore() override;
	// 更新
	virtual void Update(float elapsedTime) override;
	// 描画
	virtual void Render(const RenderContext& rc) override;
	// セッター・ゲッター
	DirectX::XMFLOAT2 GetFontOffset() const { return FontOffset; }
	void SetFontOffset(float x, float y) { FontOffset = { x, y }; }
private:
	//テクスチャ
	std::unique_ptr<Sprite> myScore;

	// --- フォント描画用 ---
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont>  spriteFont;
	std::wstring text; // 描画するテキスト

	DirectX::XMFLOAT2 FontOffset = { 10.0f, 10.0f };// フォント描画位置のオフセット
	DirectX::XMFLOAT2 fontPos{};
	DirectX::XMFLOAT2 fontScale{};
	DirectX::XMVECTORF32 fontColor = DirectX::Colors::White;

	// 画像の元のサイズ
	const float Origin_W = 500.0f;
	const float Origin_H = 215.0f;

	float scaleMag = 4.0f; // フォントの拡大率（UIのスケールに合わせて調整）
};
