#pragma once
#include <DirectXMath.h>

class Camera
{
private:
    Camera() {}
    ~Camera() {}

public:
    static Camera& Instance()
    {
        static Camera instance;
        return instance;
    }

    // ビュー
    void SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up);

    // プロジェクション
    void SetPerspectiveFov(float forY, float aspect, float nearZ, float farZ);

    // 更新（シェイク用）
    void Update(float elapsedTime);

    // シェイク開始
    void StartShake(float power, float time);

    // Getter
    const DirectX::XMFLOAT3& GetRight() const { return right; }
    const DirectX::XMFLOAT3& GetUp() const { return up; }
    const DirectX::XMFLOAT3& GetFront() const { return front; }
    const DirectX::XMFLOAT4X4& GetView() const { return view; }
    const DirectX::XMFLOAT4X4& GetProjection() const { return projection; }
private:
    DirectX::XMFLOAT4X4 view;
    DirectX::XMFLOAT4X4 projection;

    DirectX::XMFLOAT3 eye;
    DirectX::XMFLOAT3 focus;

    DirectX::XMFLOAT3 right;
    DirectX::XMFLOAT3 up;
    DirectX::XMFLOAT3 front;

    // ===== シェイク用 =====
    float shakeTime = 0.0f;
    float shakePower = 0.0f;
};