#include "EnemyManager.h"
#include "Collision.h"
#include "EnemySlime.h"
#include <imgui.h>
#include <SceneGame.h>

//エネミー削除
void EnemyManager::Remove(Enemy* enemy)
{
	_ASSERT_EXPR(enemy != nullptr, L"EnemyManager::Remove : 削除しようとしたポインタがNULLです");
	if (enemy == nullptr) return;
	//破棄リストに追加
	removes.insert(enemy);

	auto it = delayRemoves.find(enemy);
	if (it != delayRemoves.end())
	{
		delayRemoves.erase(it);
	}

}

//更新処理
void EnemyManager::Update(float elapsedTime)
{
	// 遅延削除処理
	for (auto it = delayRemoves.begin(); it != delayRemoves.end(); )
	{
		_ASSERT_EXPR(it->first != nullptr, L"delayRemovesの中にNULLポインタが含まれています");

		it->second -= elapsedTime; // 残り時間を減らす
		if (it->second <= 0.0f) // 時間が来たら削除
		{
			removes.insert(it->first);// 破棄リストに追加
			it = delayRemoves.erase(it); // マップから削除
		}
		else
		{
			it->first->Update(elapsedTime);
			++it; // 次の要素へ
		}
	}


	for (Enemy* enemy : enemies)
	{
		if (enemy == nullptr ||
			removes.find(enemy) != removes.end() ||
			delayRemoves.find(enemy) != delayRemoves.end()) // 廃棄リストに入っている場合は以降の処理をスキップ
		{
			continue;
		}

		enemy->Update(elapsedTime);

		// ゴミ処理(仮置き)
		enemy->gomiTimer += elapsedTime;
		if (enemy->gomiTimer >= 5.0f && fabs(enemy->GetPosition().y) < 0.01f)
		{
			enemy->gomiTimer = 0.0f;
			SceneGame::Instance().AddGomi(enemy->GetPosition());
		}

		// デコイに対する反応
		if (enemy->isAttracting)
		{
			// 反応処理
			ReactToDecoy(enemy, elapsedTime);
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
	if (enemies.size() < maxEnemies) {
		while (enemies.size() < maxEnemies)
		{
			Enemy* newEnemy = new EnemySlime(); // 例としてEnemySlimeを生成
			newEnemy->SetPosition(respawnPoint); // 再出現位置に配置
			Register(newEnemy); // エネミーマネージャーに登録
		}
	}

	//ステージモデルとの衝突判定
	//for(Enemy* enemy : enemies)
	//{
	//	if (enemy == nullptr ||
	//		removes.find(enemy) != removes.end() ||
	//		delayRemoves.find(enemy) != delayRemoves.end()) // 廃棄リストに入っている場合は以降の処理をスキップ
	//	{
	//		continue;
	//	}
	//	if (enemy == nullptr) _ASSERT_EXPR(false, L"EnemyManager::Update : NULLポインタが含まれています");
	//	enemy->ResolveStageCollision(SceneGame::Instance().GetStage()->GetModel(), SceneGame::Instance().GetStage()->GetWorldTransform());
	//}

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
		//if (dist <= radius && !enemy->isAttracting)
		if (dist <= radius)
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

	// マップもクリア
	delayRemoves.clear();
	removes.clear();

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

// 指定時間後に削除する
void EnemyManager::RemoveWithDelay(Enemy* enemy, float delay)
{
	// 二重登録防止
	if (delayRemoves.find(enemy) == delayRemoves.end())
	{
		delayRemoves[enemy] = delay;
	}
}

// デコイに対する反応処理
void EnemyManager::ReactToDecoy(Enemy* enemy, float elapsedTime)
{

	DirectX::XMFLOAT3 pos = enemy->GetPosition();

	float dx = enemy->attractTarget.x - pos.x;
	float dz = enemy->attractTarget.z - pos.z;

	float len = sqrtf(dx * dx + dz * dz);

	// 到着判定
	if (len < 0.5f)
	{
		enemy->isAttracting = false;
		enemy->attractCooldown = 1.0f;
		return;
	}

	dx /= len;
	dz /= len;

	pos.x += dx * 5.0f * elapsedTime;
	pos.z += dz * 5.0f * elapsedTime;

	enemy->SetPosition(pos);

	return;

}