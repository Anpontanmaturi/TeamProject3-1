#include "battery.h"
using namespace DirectX;

//コンストラクタ
Object::Object()
{
	//ステージモデルを読み込み
	model = new Model("Data/Model/object/jyuu.mdl");


	// モデルが大きいのでスケーリング
	scale = { 0.023f, 0.023f, 0.023f };
}

Object::~Object()
{
	//ステージモデルを破壊
	delete model;
}

//更新処理
void Object::Update(float elapsedTime)
{
	//今は特にやることはない
}

//描画処理
void Object::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	// スケール・回転・平行移動を合成
	XMMATRIX S = XMMatrixScaling(scale.x, scale.y, scale.z);
	XMMATRIX R = XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(rotation.x),
		XMConvertToRadians(rotation.y),
		XMConvertToRadians(rotation.z));
	XMMATRIX T = XMMatrixTranslation(position.x, position.y, position.z);

	XMMATRIX world = S * R * T;

	DirectX::XMFLOAT4X4 transform;
	DirectX::XMStoreFloat4x4(&transform, world);

	//レンダラにモデルを描画してもらう
	renderer->Render(rc, transform, model, ShaderId::Lambert);
}