#pragma once

#include "System/Model.h"
#include "System/ModelRenderer.h"
#include <DirectXMath.h>

// ƒSƒ~ƒNƒ‰ƒX
class garakuta
{
public:
    garakuta();
    ~garakuta();

    void Init(const DirectX::XMFLOAT3& pos);
    void Update(float elapsedTime);
    void Render(const RenderContext& rc, ModelRenderer* renderer);

    void Collect();

    bool IsCollected() const { return collected; }
    const DirectX::XMFLOAT3& GetPosition() const { return position; }


    float GetRadius() const { return radius; }

private:
    Model* model = nullptr;

    DirectX::XMFLOAT3 position = { 0,0,0 };
    DirectX::XMFLOAT3 scale = { 0.2f,0.2f,0.2f };

    DirectX::XMFLOAT4X4 transform;

    bool collected = false;

    void UpdateTransform();

    float radius = 0.3f; // “–‚½‚è”»’è
};