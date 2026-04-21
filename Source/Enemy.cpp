#include "Enemy.h"
#include "EnemyManager.h"
#include "SceneGame.h"
#include "Player.h"
//破棄
void Enemy::Destroy()
{
	EnemyManager::Instance().Remove(this);
}
void Enemy::Update(float elapsedTime)
{
	using namespace DirectX;

	// 現在位置
	XMFLOAT3 pos = GetPosition();

	// プレイヤー位置
	XMFLOAT3 playerPos = Player::Instance().GetPosition();

	// 方向ベクトル
	XMVECTOR vPos = XMLoadFloat3(&pos);
	XMVECTOR vPlayer = XMLoadFloat3(&playerPos);

	XMVECTOR dir = XMVectorSubtract(vPlayer, vPos);

	// 正規化（長さを1にする）
	dir = XMVector3Normalize(dir);

	// スピード
	float speed = 2.0f;

	// 移動
	XMVECTOR move = XMVectorScale(dir, speed * elapsedTime);
	vPos = XMVectorAdd(vPos, move);

	// 戻す
	XMStoreFloat3(&pos, vPos);
	SetPosition(pos);

	// ↓ ついでにゴミ処理もここに入れてOK
	gomiTimer += elapsedTime;
	if (gomiTimer >= 5.0f)
	{
		gomiTimer = 0.0f;
		
		SceneGame::Instance().AddGomi(GetPosition());
	}
}