#include "kagu.h"
#include <DirectXMath.h>

using namespace DirectX;

// コンストラクタ
kagu::kagu()
{
    model = new Model("Data/Model/furniture/reizouko_no_tex.mdl");

    scale = { 0.03f,0.03f,0.03f };
}

// デストラクタ
kagu::~kagu()
{
    delete model;
}

// 初期化
void kagu::Init(const XMFLOAT3& pos)
{
    position = pos;
    collected = false;

    UpdateTransform();
}

// 更新
void kagu::Update(float elapsedTime)
{
    if (collected) return;

    UpdateTransform();

    model->UpdateTransform();
}

// 描画
void kagu::Render(const RenderContext& rc, ModelRenderer* renderer)
{
    if (collected) return;

    if (model == nullptr) return;

    renderer->Render(rc, transform, model, ShaderId::Lambert);
}

// 行列更新
void kagu::UpdateTransform()
{
    XMMATRIX S = XMMatrixScaling(
        scale.x,
        scale.y,
        scale.z
    );

    XMMATRIX T = XMMatrixTranslation(
        position.x,
        position.y,
        position.z
    );

    XMMATRIX world = S * T;

    XMStoreFloat4x4(&transform, world);
}

bool kagu::IsBroken() const
{
    return collected;
}

DirectX::XMFLOAT3 kagu::GetPosition() const
{
    return position;
}

float kagu::GetRadius() const
{
    return 1.0f;
}

void kagu::Break()
{
    collected = true;
}