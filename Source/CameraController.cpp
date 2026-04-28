#include"System/Input.h"
#include"CameraController.h"
#include"Camera.h"

//更新処理
void CameraController::Update(float elapsedTime)
{
	bool currentKeyTab = (GetAsyncKeyState(VK_TAB) & 0x8000);

	if (currentKeyTab && !prevKeyTab)
	{
		isCursorLocked = !isCursorLocked; // フラグ反転

		// カーソルを表示/非表示にする（お好みで）
		ShowCursor(!isCursorLocked);

		// カーソルをロックした場合に画面中央に移動させる
		if (isCursorLocked)
		{
			SetCursorPos(centerX, centerY);
		}

	}
	prevKeyTab = currentKeyTab;

	// カーソルがロックされている場合のみマウス移動を処理
	if (isCursorLocked)
	{

		// 現在のマウス座標取得
		POINT mousePos;
		GetCursorPos(&mousePos);

		// 相対移動量
		float ax = static_cast<float>(mousePos.x - centerX);
		float ay = static_cast<float>(mousePos.y - centerY);

		// 角度更新
		angle.y += ax * sensitivity;

		//// X軸回転制限
		//if (angle.x < minAngleX) angle.x = minAngleX;
		//if (angle.x > maxAngleX) angle.x = maxAngleX;

		//	X軸回転を固定
		angle.x = DirectX::XMConvertToRadians(20.0f);

		// Y軸回転ラップ
		if (angle.y < -DirectX::XM_PI) angle.y += DirectX::XM_2PI;
		if (angle.y > DirectX::XM_PI) angle.y -= DirectX::XM_2PI;

		// マウスを画面中央に戻す
		SetCursorPos(centerX, centerY);
	}

	// ズーム処理
	int wheel = Input::Instance().GetMouseWheel();

	if (wheel != 0)
	{
		range -= static_cast<float>(wheel) * zoomSpeed;

		if (range < minRange)  range = minRange;  // キャラクターにめり込まない距離
		if (range > maxRange) range = maxRange; // ステージの外に出すぎない距離
	}

	//カメラ回転値を回転行列に変換	
	Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);


	//カメラ回転値を回転行列に変換
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	

	//回転行列から前方向ベクトルを取り出す
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	//注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
	DirectX::XMFLOAT3 eye;
	eye.x = target.x - front.x * range;
	eye.y = target.y - front.y * range;
	eye.z = target.z - front.z * range;

	//カメラの視点と注視点を設定
	Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}