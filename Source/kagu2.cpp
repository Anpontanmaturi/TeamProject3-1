#include "kagu2.h"
#include <DirectXMath.h>

using namespace DirectX;

// コンストラクタ
kagu2::kagu2()
{
    model = new Model("Data/Model/kagu/bed_notex.mdl");

    scale = { 0.03f, 0.03f, 0.03f };

    // 四隅コライダー（ベッド基準）
    legs =
    {
        { -1.0f, 0.0f,  1.0f, 0.3f }, // 左奥
        {  1.0f, 0.0f,  1.0f, 0.3f }, // 右奥
        { -1.0f, 0.0f, -1.0f, 0.3f }, // 左手前
        {  1.0f, 0.0f, -1.0f, 0.3f }, // 右手前
    };
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
    collected = false;

    UpdateTransform();
}

// 更新
void kagu2::Update(float elapsedTime)
{
    if (collected) return;

    UpdateTransform();

    if (model)
    {
        model->UpdateTransform();
    }
}

// 描画
void kagu2::Render(const RenderContext& rc, ModelRenderer* renderer)
{
    if (collected) return;
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

    // ★重要：位置ズレの原因を削除（-5.5なし）
    XMMATRIX T = XMMatrixTranslation(
        position.x,
        position.y,
        position.z
    );

    XMMATRIX world = S * T;

    XMStoreFloat4x4(&transform, world);
}

// 状態
bool kagu2::IsBroken() const
{
    return collected;
}

// 位置取得
DirectX::XMFLOAT3 kagu2::GetPosition() const
{
    return position;
}

// 破壊
void kagu2::Break()
{
    collected = true;
}

// 四隅コライダー取得
const std::vector<kagu2::LegCollider>& kagu2::GetLegs() const
{
    return legs;
}