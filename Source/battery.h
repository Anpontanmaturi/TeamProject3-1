#pragma once
#include "system/Model.h"
#include "system/ModelRenderer.h"
#include <DirectXMath.h>
//ステージ
class Object
{
public:
	Object();
	~Object();

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(const RenderContext& rc, ModelRenderer* rendere);

	// スケール・回転・位置を外から設定できるように
	void SetScale(float x, float y, float z) { scale = { x, y, z }; }
	void SetRotation(float x, float y, float z) { rotation = { x, y, z }; }
	void SetPosition(float x, float y, float z) { position = { x, y, z }; }
private:
	Model* model;

	DirectX::XMFLOAT3 scale{ 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT3 rotation{ 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 position{ 0.0f, 0.0f, 0.0f };

};
