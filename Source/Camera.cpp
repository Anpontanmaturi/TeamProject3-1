#include "Camera.h"
#include <cstdlib>

// シェイク開始
void Camera::StartShake(float power, float time)
{
    shakePower = power;
    shakeTime = time;
}

// 更新
void Camera::Update(float elapsedTime)
{
    if (shakeTime > 0.0f)
    {
        shakeTime -= elapsedTime;

        // 徐々に弱くする
        shakePower *= 0.9f;
    }
}

// 指定方向を向く
void Camera::SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up)
{
    DirectX::XMFLOAT3 shakenEye = eye;
    DirectX::XMFLOAT3 shakenFocus = focus;

    // ===== シェイク処理 =====
    if (shakeTime > 0.0f)
    {
        float offsetX = ((rand() % 100) / 100.0f - 0.5f) * shakePower;
        float offsetY = ((rand() % 100) / 100.0f - 0.5f) * shakePower;

        shakenEye.x += offsetX;
        shakenEye.y += offsetY;

        shakenFocus.x += offsetX;
        shakenFocus.y += offsetY;
    }

    // ビュー行列作成
    DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&shakenEye);
    DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&shakenFocus);
    DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);

    DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
    DirectX::XMStoreFloat4x4(&view, View);

    // カメラ方向
    this->right.x = view._11;
    this->right.y = view._21;
    this->right.z = view._31;

    this->up.x = view._12;
    this->up.y = view._22;
    this->up.z = view._32;

    this->front.x = view._13;
    this->front.y = view._23;
    this->front.z = view._33;

    this->eye = eye;
    this->focus = focus;
}

// パース設定
void Camera::SetPerspectiveFov(float forY, float aspect, float nearZ, float farZ)
{
    DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(forY, aspect, nearZ, farZ);
    DirectX::XMStoreFloat4x4(&projection, Projection);
}