#pragma once

#include "System/ModelRenderer.h"
#include "Character.h"

//エネミー
class Enemy : public Character
{
public:
	Enemy(){}
	~Enemy()override{}
	virtual void Update(float elapsedTime);
	//破棄
	void Destroy();
	float gomiTimer = 0.0f;
	//更新処理
	

	//描画処理
	virtual void Render(const RenderContext& rc, ModelRenderer* renderer) = 0;
};
