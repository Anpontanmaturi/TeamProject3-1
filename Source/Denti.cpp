#include "Gomi.h"
#include <DirectXMath.h>
#include"Denti.h"
using namespace DirectX;

// コンストラクタ
Denti::Denti()
{
    model = new Model("Data/Model/denti/denti.mdl");

   

    // サイズ調整（必要なら）
    scale = { 0.02f, 0.02f, 0.02f };
}

// デストラクタ
Denti::~Denti()
{
    delete model;
}

// 初期化
void Denti::Init(const XMFLOAT3& pos)
{
    position = pos;
    collected = false;

    UpdateTransform();
}

// 更新
void Denti::Update(float elapsedTime)
{
    if (collected) return;

    UpdateTransform();
    model->UpdateTransform();
}

// 描画
void Denti::Render(const RenderContext& rc, ModelRenderer* renderer)
{
    if (collected) return;
    if (model == nullptr) return;
    renderer->Render(rc, transform, model, ShaderId::Lambert);
}

// 回収
void Denti::Collect()
{
    collected = true;
}

// 行列更新
void Denti::UpdateTransform()
{
    XMMATRIX S = XMMatrixScaling(scale.x, scale.y, scale.z);
    XMMATRIX T = XMMatrixTranslation(position.x, position.y, position.z);

    XMMATRIX world = S * T;

    XMStoreFloat4x4(&transform, world);
}