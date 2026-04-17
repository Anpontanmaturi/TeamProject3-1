#pragma once

#include"System/ModelRenderer.h"
#include"Character.h"
#include"ProjectileManager.h"

//プレイヤー
class Player : public Character
{
/*public:
	Player();
	~Player() override;*/
private:
	Player() {};
	~Player() override {};

public:
	//インスタンス取得
	static Player& Instance()
	{
		static Player instance;
		return instance;
	}

	//初期化
	void Initialize();

	//終了化
	void Finalize();

	//更新処理
	void Update(float elapsedTime);

	//デバッグ用GUI描画
	void DrowDebugGUI();

	//描画処理
	void Render(const RenderContext& rc, ModelRenderer* renderer);

	//ジャンプ処理
	//void Jump(float speed);

	//速度入力更新
	//void UpdateVelocity(float elapsedTime);

	//ジャンプ入力処理
	void InputJump();

	//デバッグプリミティブ描画
	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);

private:
	//スティック入力値から移動ベクトルを取得
	DirectX::XMFLOAT3 GetMoveVec() const;

	//移動処理
	//void Move(float elapsedTime, float vx, float vz, float speed);

	//移動入力処理
	void InputMove(float elapsedTime);

	//弾丸入力処理
	void InputProjectile();

	//旋回処理
	//void Turn(float elapsedTime, float vx, float vz, float speed);

	//プレイヤーとエネミーとの衝突処理
	void CollisionPlayerVsEnemies();

	//弾丸と敵の衝突処理
	void CollisionProjectilesVsEnemies();

protected:
	//着地した時に呼ばれる
	void OnLanding() override;

private:
	Model* model = nullptr;
	float moveSpeed = 5.0f;
	float turnSpeed = DirectX::XMConvertToRadians(720);

	float jumpSpeed = 12.0f;
	//float gravity = -30.0f;
	//DirectX::XMFLOAT3 velocity = { 0, 0, 0 };
	int jumpCount = 0;
	int jumpLimit = 2;

	ProjectileManager projectileManager;
};
