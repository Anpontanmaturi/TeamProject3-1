#include "kagu2.h"
#include <DirectXMath.h>

using namespace DirectX;

// コンストラクタ
kagu2::kagu2()
{
    model = new Model("Data/Model/kagu/tana.mdl");

    scale = { 0.02f,0.03f,0.02f };
}

// デストラクタ
kagu2::~kagu2()
{
    delete model;
}

// 初期化
void kagu2::Init(const XMFLOAT3& pos)
{
    position = pos;

    // 角度（必要なら変える）
    rotation = { 0, 298.45, 0 };

    broken = false;

    UpdateTransform();
}

// 更新
void kagu2::Update(float elapsedTime)
{
    if (broken) return;

    UpdateTransform();

    if (model)
    {
        model->UpdateTransform();
    }
}

// 描画
void kagu2::Render(const RenderContext& rc, ModelRenderer* renderer)
{
    if (broken) return;
    if (model == nullptr) return;

    renderer->Render(rc, transform, model, ShaderId::Lambert);
}

// 行列更新
void kagu2::UpdateTransform()
{
    XMMATRIX S = XMMatrixScaling(
        scale.x,
        scale.y,
        scale.z
    );

    XMMATRIX R = XMMatrixRotationRollPitchYaw(
        rotation.x,
        rotation.y,
        rotation.z
    );

    XMMATRIX T = XMMatrixTranslation(
        position.x,
        position.y,
        position.z
    );

    XMMATRIX world = S * R * T;

    XMStoreFloat4x4(&transform, world);
}

// 壊れる
void kagu2::Break()
{
    broken = true;
}

// 壊れてるか
bool kagu2::IsBroken() const
{
    return broken;
}

// 位置取得
DirectX::XMFLOAT3 kagu2::GetPosition() const
{
    return position;
}

// 当たり判定半径
float kagu2::GetRadius() const
{
    return 2.0f;
}