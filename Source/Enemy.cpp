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
    if (isAttracting)
    {
        DirectX::XMFLOAT3 pos = GetPosition();

        float dx = attractTarget.x - pos.x;
        float dz = attractTarget.z - pos.z;

        float len = sqrtf(dx * dx + dz * dz);

        // š “’…”»’è
        if (len < 0.5f)
        {
            isAttracting = false;
            attractCooldown = 1.0f;
            return;
        }

        dx /= len;
        dz /= len;

        pos.x += dx * 5.0f * elapsedTime;
        pos.z += dz * 5.0f * elapsedTime;

        SetPosition(pos);

        return;
    }
}
void Enemy::StartAttract(const DirectX::XMFLOAT3& targetPos)
{
    isAttracting = true;
    attractTarget = targetPos;
}