#include "Enemy.h"
#include "EnemyManager.h"

//”jŠü
void Enemy::Destroy()
{
	EnemyManager::Instance().Remove(this);
}