#pragma once

#include "System/ModelRenderer.h"
#include "Character.h"

//エネミー
class Enemy : public Character
{
public:
	Enemy(){}
	~Enemy()override{}
	virtual void Update(float elapsedTime) = 0;
	void StartAttract(const DirectX::XMFLOAT3& target);
	//破棄
	void Destroy();
	float gomiTimer = 0.0f;
	//更新処理
	bool isAttracting = false;
	DirectX::XMFLOAT3 attractTarget;

	//描画処理
	virtual void Render(const RenderContext& rc, ModelRenderer* renderer) = 0;
};
