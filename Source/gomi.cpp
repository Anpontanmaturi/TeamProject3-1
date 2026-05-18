#include "Gomi.h"
#include <DirectXMath.h>

using namespace DirectX;

// コンストラクタ
Gomi::Gomi()
{
    model = new Model("Data/Model/hoko/hoko.mdl");

    // サイズ調整（必要なら）
    scale = { 0.075f, 0.075f, 0.075f };
}

// デストラクタ
Gomi::~Gomi()
{
    delete model;
}

// 初期化
void Gomi::Init(const XMFLOAT3& pos)
{
    position = pos;
    collected = false;

    UpdateTransform();
}

// 更新
void Gomi::Update(float elapsedTime)
{
    if (collected) return;

    UpdateTransform();
    model->UpdateTransform();
}

// 描画
void Gomi::Render(const RenderContext& rc, ModelRenderer* renderer)
{
    if (collected) return;
    if (model == nullptr) return;
    renderer->Render(rc, transform, model, ShaderId::Lambert);
}

// 回収
void Gomi::Collect()
{
    collected = true;
}

// 行列更新
void Gomi::UpdateTransform()
{
    XMMATRIX S = XMMatrixScaling(scale.x, scale.y, scale.z);
    XMMATRIX T = XMMatrixTranslation(position.x, position.y, position.z);

    XMMATRIX world = S * T;

    XMStoreFloat4x4(&transform, world);
}