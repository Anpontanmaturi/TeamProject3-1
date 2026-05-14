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

private:
	// テクスチャ
	std::unique_ptr<Sprite> counter;

	// --- フォント描画用 ---
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont>  spriteFont;

	const DirectX::XMFLOAT2 FontOffset = { 220.0f, 110.0f };
	DirectX::XMFLOAT2 fontScale;

	// 画像の元のサイズ
	const float Origin_W = 500.0f;
	const float Origin_H = 200.0f;


};