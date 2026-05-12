#include "garakuta.h"
#include <DirectXMath.h>

using namespace DirectX;

// コンストラクタ
garakuta::garakuta()
{
    model = new Model("Data/Model/SpikeBall/SpikeBall.mdl");

    // サイズ調整（必要なら）
    scale = { 0.6f, 0.6f, 0.6f };
}

// デストラクタ
garakuta::~garakuta()
{
    delete model;
}

// 初期化
void garakuta::Init(const XMFLOAT3& pos)
{
    position = pos;
    collected = false;

    UpdateTransform();
}

// 更新
void garakuta::Update(float elapsedTime)
{
    if (collected) return;

    UpdateTransform();
    model->UpdateTransform();
}

// 描画
void garakuta::Render(const RenderContext& rc, ModelRenderer* renderer)
{
    if (collected) return;
    if (model == nullptr) return;
    renderer->Render(rc, transform, model, ShaderId::Lambert);
}

// 回収
void garakuta::Collect()
{
    collected = true;
}

// 行列更新
void garakuta::UpdateTransform()
{
    XMMATRIX S = XMMatrixScaling(scale.x, scale.y, scale.z);
    XMMATRIX T = XMMatrixTranslation(position.x, position.y, position.z);

    XMMATRIX world = S * T;

    XMStoreFloat4x4(&transform, world);
}