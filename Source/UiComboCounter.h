#pragma once
#include "UiBase.h"
#include "System/Sprite.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <string>
#include <memory>

class UIComboCounter : public UIBase
{
public:
	UIComboCounter();
	virtual ~UIComboCounter() override;
	// 更新
	virtual void Update(float elapsedTime) override;
	// 描画
	virtual void Render(const RenderContext& rc) override;

	// セッター・ゲッター
	DirectX::XMFLOAT2 GetFontOffset() const { return FontOffset; }
	void SetFontOffset(float x, float y) { FontOffset = { x, y }; }

private:
	// テクスチャ
	std::unique_ptr<Sprite> combo;

	// --- フォント描画用 ---
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont>  spriteFont;

	DirectX::XMFLOAT2 FontOffset = { 110.0f, 90.0f };
	DirectX::XMFLOAT2 fontPos{};
	DirectX::XMFLOAT2 fontScale{};
	DirectX::XMVECTORF32 fontColor = DirectX::Colors::Black;
	std::wstring text; // 描画するテキスト
	int lastCount = -1; // 前回のカウント値（初期値は-1）

	// 画像の元のサイズ
	const float Origin_W = 190.0f;
	const float Origin_H = 190.0f;

	float scaleMag = 1.5f; // フォントの拡大率（UIのスケールに合わせて調整）
};
