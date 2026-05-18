#pragma once
#include "UiBase.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <memory>
#include <string>
#include <vector>

class UiRanking : public UIBase
{
public:
	UiRanking();
	virtual ~UiRanking() override;
	// 更新
	virtual void Update(float elapsedTime) override;
	// 描画
	virtual void Render(const RenderContext& rc) override;
	// セッター・ゲッター
	DirectX::XMFLOAT2 GetFontOffset() const { return FontOffset; }
	void SetFontOffset(float x, float y) { FontOffset = { x, y }; }

	// 終了化
	void Finalize() {
		spriteBatch.reset();
		spriteFont.reset();
	}
private:
	// --- フォント描画用 ---
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont>  spriteFont;

	std::vector<std::wstring> rankingTexts; // 5位までのランキングテキストを格納
	std::wstring m_currentScoreText; // 今回のスコア用テキスト

	DirectX::XMFLOAT2 FontOffset = { 0.0f, 0.0f }; // フォント描画位置のオフセット
	DirectX::XMFLOAT2 fontPos{};
	DirectX::XMFLOAT2 fontScale{};
	DirectX::XMVECTORF32 fontColor = DirectX::Colors::White;
	float scaleMag = 3.0f; // フォントの拡大率（UIのスケールに合わせて調整）

	float lineSpace = 120.0f;// 行間
};