#pragma once

#include "System/ModelRenderer.h"
#include "System/Model.h"
#include "Collision.h"

//ステージ
class Stage
{
public:
	Stage();
	~Stage();

	// インスタンス取得
	static Stage& Instance();

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(const RenderContext& rc, ModelRenderer* renderer);

	/*const Model* GetModel() const { return model; }
	Model* GetModel() { return model; }

	const DirectX::XMFLOAT4X4& GetWorldTransform() const { return worldTransform; }*/

	// レイキャスト
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);


private:
	Model* model = nullptr;
};
