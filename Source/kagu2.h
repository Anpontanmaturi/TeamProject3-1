#pragma once
#include "System/Model.h"
#include "System/ModelRenderer.h"
#include <DirectXMath.h>

class kagu2
{
public:
    kagu2();
    ~kagu2();

    void Init(const DirectX::XMFLOAT3& pos);
    void Update(float elapsedTime);
    void Render(const RenderContext& rc, ModelRenderer* renderer);

    void Break();
    bool IsBroken() const;

    DirectX::XMFLOAT3 GetPosition() const;
    float GetRadius() const;

private:
    void UpdateTransform();

private:
    Model* model = nullptr;

    DirectX::XMFLOAT3 position = { 0,0,0 };
    DirectX::XMFLOAT3 scale = { 1,1,1 };
    DirectX::XMFLOAT3 rotation = { 0,0,0 };

    DirectX::XMFLOAT4X4 transform;

    bool broken = false;
};