#pragma once
#include "UiBase.h"
#include "System/Sprite.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
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

	// 調整用
	void SetDustPosition(float x, float y) { DustUI_position = DirectX::XMFLOAT2(x, y); }
	void SetDustScale(float x, float y) { DustUI_scale = DirectX::XMFLOAT2(x, y); }

private:
	// テクスチャ
	std::unique_ptr<Sprite> counter;

	// --- フォント描画用 ---
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont>  spriteFont;

	const DirectX::XMFLOAT2 FontOffset = { 150.0f, 50.0f };

	// 画像の元のサイズ
	const float Origin_W = 500.0f;
	const float Origin_H = 200.0f;

	DirectX::XMFLOAT2 DustUI_position;
	DirectX::XMFLOAT2 DustUI_scale;

};