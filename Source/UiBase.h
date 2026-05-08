#pragma once
#include <DirectXMath.h>
#include <System/RenderContext.h>

class UIBase
{
public:
	UIBase() : position(0, 0), scale(1, 1), color(1, 1, 1, 1), isVisible(true) {};
	virtual ~UIBase() {};

	// 更新・描画
	virtual void Update(float elapsedTime) = 0;
	virtual void Render(const RenderContext& rc) = 0;

	// その他セッター等
	void SetPosition(float x, float y) { position.x = x; position.y = y; }
	const DirectX::XMFLOAT2& GetPosition() const { return position; }

	void SetScale(float x, float y) { scale.x = x; scale.y = y; }
	const DirectX::XMFLOAT2& GetScale() const { return scale; }
	void SetColor(float r, float g, float b, float a) { color = { r, g, b, a }; }

	void SetVisible(bool visible) { isVisible = visible; }
	bool IsVisible() const { return isVisible; }

protected:
	DirectX::XMFLOAT2 position; // 座標
	DirectX::XMFLOAT2 scale;	// サイズ・拡大率
	DirectX::XMFLOAT4 color;	// 色と透明度
	bool isVisible;				// 表示フラグ
};

