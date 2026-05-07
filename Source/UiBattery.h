#pragma once
#include "UiBase.h"
#include "System/Sprite.h"
#include <memory>

class UIBattery : public UIBase
{
public:
	UIBattery();
	virtual ~UIBattery() override;

	// 更新
	virtual void Update(float elapsedTime) override;
	// 描画
	virtual void Render(const RenderContext& rc) override;

private:
	// テクスチャ
	std::unique_ptr<Sprite> frame;      // 電池の枠
	std::unique_ptr<Sprite> fill;       // 中身
	std::unique_ptr<Sprite> back;		// 背景

	float energy = 1.0f; // エネルギー量（0.0～1.0）

	// 画像の元のサイズ
	const float baseWidth = 500.0f;
	const float baseHeight = 144.0f;
};