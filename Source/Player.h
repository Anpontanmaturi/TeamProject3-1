#pragma once

#include"System/ModelRenderer.h"
#include"Character.h"
#include <System/AudioSource.h>

//プレイヤー
class Player : public Character
{
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
	void AddEnergy(float value);

	void AddGarbage(int value);

	//終了化
	void Finalize();

	//更新処理
	void Update(float elapsedTime);

	//デバッグ用GUI描画
	void DrowDebugGUI();

	//描画処理
	void Render(const RenderContext& rc, ModelRenderer* renderer);

	// 回復処理
	void Heal(float amount);

	// バッテリーのゲッター・セッター
	float GetEnergy() const { return energy; }
	void SetEnergy(float value) { energy = value; }
	float GetMaxEnergy() const { return maxenergy; }

	// 現在のガラクタ所持数を取得する
	int GetGarbageCount() const { return garbageCount; }

	//ジャンプ入力処理
	void InputJump();

	//デバッグプリミティブ描画
	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);


	void Reset();
private:
	//スティック入力値から移動ベクトルを取得
	DirectX::XMFLOAT3 GetMoveVec() const;

	//移動入力処理
	void InputMove(float elapsedTime);

	//弾丸入力処理
	void InputProjectile();

	//HP取得
	float GetHp() const { return hp; }
	float GetMaxHp() const { return maxHp; }


	//旋回処理
	//void Turn(float elapsedTime, float vx, float vz, float speed);

	//プレイヤーとエネミーとの衝突処理
	void CollisionPlayerVsEnemies();

	//弾丸と敵の衝突処理
	void CollisionProjectilesVsEnemies();

	//充電中動けないようにする
	bool canMove = true;

protected:
	//着地した時に呼ばれる
	void OnLanding() override;

private:
	Model* model = nullptr;
	float moveSpeed = 5.0f;

	float turnSpeed = DirectX::XMConvertToRadians(720);

	float hp = 100.0f;
	float maxHp = 100.0f;

	float jumpSpeed = 12.0f;
	//float gravity = -30.0f;
	//DirectX::XMFLOAT3 velocity = { 0, 0, 0 };
	int jumpCount = 0;
	int jumpLimit = 2;
	//追加
	float moveLimit = 5.5f;
	float boostLimit = 8.0f;
	float energy = 1000.0f;
	float maxenergy = 1000.0f;
	
	int garbageCount = 0; // ←これ追加!!

	float deleteEnemyTimer = 0.5f; //敵削除タイマー

private:
	bool isBoost = false;

public:
	bool IsBoost() const { return isBoost; }

	int gomiCount = 0;

	void SetCanMove(bool flag) { canMove = flag; }
	bool CanMove() const { return canMove; }

	AudioSource* PSE1 = nullptr;//ブースト音
	AudioSource* PSE2 = nullptr;//ジャンプ音
	AudioSource* PSE3 = nullptr;//攻撃音

	//========================================
	// 向き取得
	//========================================
	const DirectX::XMFLOAT3& GetAngle() const
	{
		return angle;
	}

	//========================================
	// 向き設定
	//========================================
	void SetAngle(const DirectX::XMFLOAT3& a)
	{
		angle = a;
	}
};
