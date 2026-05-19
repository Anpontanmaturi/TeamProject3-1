#include "Stage.h"

// インスタンス取得
Stage& Stage::Instance()
{
	static Stage instance;
	return instance;
}

//コンストラクタ
Stage::Stage()
{
	//ステージモデルを読み込み
	model = new Model("Data/Model/Stage/map.mdl");
	//model = new Model("Data/Model/ExampleStage/ExampleStage.mdl");
}

//デストラクタ
Stage::~Stage()
{
	//ステージモデルを破棄
	delete model;
}

//更新処理
void Stage::Update(float elapsedTime)
{
	//今はなし
}

//描画処理
void Stage::Render(const RenderContext& rc, ModelRenderer* renderer)
{

	// 拡大率
	//float scale = 0.025f;
	float scale = 1.00f;

	// 位置
	float posX = 0.0f;
	float posY = 0.0f;
	float posZ = 0.0f;

	// スケール行列
	DirectX::XMMATRIX S =
		DirectX::XMMatrixScaling(scale, scale, scale);

	// 移動行列
	DirectX::XMMATRIX T =
		DirectX::XMMatrixTranslation(posX, posY, posZ);

	// 合成行列
	DirectX::XMMATRIX world = S * T;

	// float4x4へ変換
	DirectX::XMFLOAT4X4 transform;
	DirectX::XMStoreFloat4x4(&transform, world);

	// 描画
	renderer->Render(rc, transform, model, ShaderId::Lambert);
}

// レイキャスト
bool Stage::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	return Collision::IntersectRayVsModel(start, end, model, hit);
}