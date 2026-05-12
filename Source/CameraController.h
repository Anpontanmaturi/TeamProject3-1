#pragma once

#include<DirectXMath.h>

//カメラコントローラー

class CameraController

{

public:

	//更新処理

	void Update(float elapsedtime);

	//ターゲット位置設定

	void SetTarget(const DirectX::XMFLOAT3/*&*/ t)

	{

		target = t;

	}

private:

	DirectX::XMFLOAT3	target = { 0,0,0 };//注視点

	DirectX::XMFLOAT3	angle = { 0,0,0 }; //回転角度

	DirectX::XMMATRIX	Transform;

	float				rollSpeed = DirectX::XMConvertToRadians(90);//回転速度

	float				range = 10.0f; //距離

	float				maxAngleX = DirectX::XMConvertToRadians(45);

	float				minAngleX = DirectX::XMConvertToRadians(-45);

	float sensitivity = 0.001f; //マウス感度

	float screenWidth = 1280; //画面幅

	float screenHeight = 720; //画面高さ

	// 画面中央座標

	int centerX = screenWidth / 2;

	int centerY = screenHeight / 2;

	bool isCursorLocked = true; // カーソルのロック状態

	bool prevKeyTab = false;    // 前フレームのキー状態

	const float zoomSpeed = 0.01f;    // ズームの感度

	const float minRange = 2.0f;     // 最接近距離

	const float maxRange = 15.0f;    // 最大遠方距離

};

