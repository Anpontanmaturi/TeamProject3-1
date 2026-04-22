#include "EnemyManager.h"
#include "Collision.h"
#include "EnemySlime.h"
#include <imgui.h>

//エネミー削除
void EnemyManager::Remove(Enemy* enemy)
{
	//破棄リストに追加
	removes.insert(enemy);
}

//更新処理
void EnemyManager::Update(float elapsedTime)
{

	for (Enemy* enemy : enemies)
	{
		if (enemy != nullptr && removes.find(enemy) == removes.end())//破棄リストにない場合のみ更新処理を行う
		{
			enemy->Update(elapsedTime);
		}
	}

	//破棄処理
	//※enemysの範囲for文中でerase()すると不具合が発生してしまうため、
	//　更新処理が終わった後に破棄リストに積まれたオブジェクトを削除する。
	for (Enemy* enemy : removes)
	{
		//std::vectorから要素を削除する場合はイテレーターで削除しなければならない
		std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);
		if (it != enemies.end())
		{
			enemies.erase(it);
		}

		//削除
		delete enemy;
	}

	//廃棄リストをクリア
	removes.clear();

	// エネミーの補充処理
	while(enemies.size() < maxEnemies)
	{
		Enemy* newEnemy = new EnemySlime(); // 例としてEnemySlimeを生成
		newEnemy->SetPosition(respawnPoint); // 再出現位置に配置
		Register(newEnemy); // エネミーマネージャーに登録
	}

	//敵同士の衝突処理
	CollisionEnemyVsEnemies();
}

//描画処理
void EnemyManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Render(rc, renderer);
	}
}

//デバッグプリミティブ描画
void EnemyManager::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	for (Enemy* enemy : enemies)
	{
		enemy->RenderDebugPrimitive(rc, renderer);
	}
}

void EnemyManager::AttractEnemies(const DirectX::XMFLOAT3& center, float radius)
{
	for (Enemy* enemy : enemies)
	{
		DirectX::XMFLOAT3 pos = enemy->GetPosition();

		float dx = pos.x - center.x;
		float dz = pos.z - center.z;

		float dist = sqrtf(dx * dx + dz * dz);
		if (dist <= radius && !enemy->isAttracting)
		{
			DirectX::XMFLOAT3 target = center;
			target.z += 2.0f;

			enemy->StartAttract(target);
		}
	}
}

//エネミー登録
void EnemyManager::Register(Enemy* enemy)
{
	enemies.emplace_back(enemy);
}

//エネミー全削除
void EnemyManager::Clear()
{
	for (Enemy* enemy : enemies)
	{
		delete enemy;
	}
	enemies.clear();
}

//エネミー同士の衝突処理
void EnemyManager::CollisionEnemyVsEnemies()
{
	size_t enemyCount = enemies.size();
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemyA = enemies.at(i);
		for (int j = i + 1; j < enemyCount; ++j)
		{
			Enemy* enemyB = enemies.at(j);

			DirectX::XMFLOAT3 outPosition;
			/*if (Collision::IntersectSphereVsSphere(
				enemyA->GetPosition(),
				enemyA->GetRadius(),
				enemyB->GetPosition(),
				enemyB->GetRadius(),
				outPosition
			))
			{
				enemyB->SetPosition(outPosition);
			}*/
			if (Collision::IntersectCylinderVsCylinder(
				enemyA->GetPosition(),
				enemyA->GetRadius(),
				enemyA->GetHeight(),
				enemyB->GetPosition(),
				enemyB->GetRadius(),
				enemyB->GetHeight(),
				outPosition
			))
			{
				enemyB->SetPosition(outPosition);
			}
		}
	}
}

void EnemyManager::DrawDebugGUI()
{
	ImGui::Begin("Enemy Manager");
	ImGui::End();
}