#include "Enemy.h"
#include "EnemyManager.h"
#include "SceneGame.h"
#include "Player.h"
//”jŠü
void Enemy::Destroy()
{
	EnemyManager::Instance().Remove(this);
}
void Enemy::Update(float elapsedTime)
{
	using namespace DirectX;

	// ===== ‹zˆø’† =====
	if (isAttracting)
	{
		XMFLOAT3 pos = GetPosition();

		XMVECTOR vPos = XMLoadFloat3(&pos);
		XMVECTOR vTarget = XMLoadFloat3(&attractTarget);

		XMVECTOR dir = XMVectorSubtract(vTarget, vPos);

		float dist = XMVectorGetX(XMVector3Length(dir));

		if (dist < 0.5f)
		{
			isAttracting = false;
		}
		else
		{
			dir = XMVector3Normalize(dir);

			float speed = 2.0f; // © ­‚µã‚°‚½•û‚ªŽ©‘R

			XMVECTOR move = XMVectorScale(dir, speed * elapsedTime);
			vPos = XMVectorAdd(vPos, move);

			XMStoreFloat3(&pos, vPos);
			SetPosition(pos);
		}

		return; // © š‚±‚ê’´d—vII
	}

	// ===== ’ÊíˆÚ“® =====
	XMFLOAT3 pos = GetPosition();
	XMFLOAT3 playerPos = Player::Instance().GetPosition();

	XMVECTOR vPos = XMLoadFloat3(&pos);
	XMVECTOR vPlayer = XMLoadFloat3(&playerPos);

	XMVECTOR dir = XMVectorSubtract(vPlayer, vPos);
	dir = XMVector3Normalize(dir);

	float speed = 2.0f;

	XMVECTOR move = XMVectorScale(dir, speed * elapsedTime);
	vPos = XMVectorAdd(vPos, move);

	XMStoreFloat3(&pos, vPos);
	SetPosition(pos);

	// ƒSƒ~ˆ—
	gomiTimer += elapsedTime;
	if (gomiTimer >= 5.0f)
	{
		gomiTimer = 0.0f;
		SceneGame::Instance().AddGomi(GetPosition());
	}
}
void Enemy::StartAttract(const DirectX::XMFLOAT3& target)
{
	isAttracting = true;
	attractTarget = target;
}