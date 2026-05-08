#include "Enemy.h"
#include "EnemyManager.h"
#include "SceneGame.h"
#include "Player.h"
//”jŠü
void Enemy::Destroy()
{
	EnemyManager::Instance().Remove(this);
}

void Enemy::StartAttract(const DirectX::XMFLOAT3& target)
{
	isAttracting = true;
	attractTarget = target;
}