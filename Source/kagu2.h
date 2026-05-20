#pragma once
#include "System/Model.h"
#include "System/ModelRenderer.h"
#include <DirectXMath.h>
#include <vector>
class kagu2
{
public:
    kagu2();
    ~kagu2();

    void Init(const DirectX::XMFLOAT3& pos);
    void Update(float elapsedTime);
    void Render(const RenderContext& rc, ModelRenderer* renderer);

    bool IsBroken() const;
    void Break();

    DirectX::XMFLOAT3 GetPosition() const;

    struct LegCollider
    {
        float offsetX;
        float offsetY;
        float offsetZ;
        float radius;   // Åöí«â¡
    };

    const std::vector<LegCollider>& GetLegs() const;

private:
    void UpdateTransform();
    std::vector<LegCollider> legs;
private:
    Model* model = nullptr;

    DirectX::XMFLOAT3 position = { 0,0,0 };
    DirectX::XMFLOAT3 scale = { 1,1,1 };

    DirectX::XMFLOAT4X4 transform;

    bool collected = false;
};