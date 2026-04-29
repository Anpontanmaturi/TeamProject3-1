#pragma once

#include <vector>
#include "Enemy.h"
#include <set>
#include <map>

//エネミーマネージャー
class EnemyManager
{
private:
	EnemyManager(){}
	~EnemyManager(){}

public:
	//唯一のインスタンス取得
	static EnemyManager& Instance()
	{
		static EnemyManager instance;
		return instance;
	}
	void AttractEnemies(const DirectX::XMFLOAT3& center, float radius);
	//エネミー登録
	void Register(Enemy* enemy);

	//エネミー数取得
	int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

	//エネミー取得
	Enemy* GetEnemy(int index) { return enemies.at(index); }

	//エネミー削除
	void Remove(Enemy* enemy);

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(const RenderContext& rc, ModelRenderer* renderer);

	//デバッグプリミティブ描画
	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);

	//エネミー用GUI表示
	void DrawDebugGUI();

	//敵を消す処理
	void RemoveWithDelay(Enemy* enemy, float delay);

	//デコイに対する反応処理
	void ReactToDecoy(Enemy* enemy, float elapsedTime);

	//エネミー全削除
	void Clear();

private:
	//エネミー同士の衝突処理
	void CollisionEnemyVsEnemies();

	std::vector<Enemy*>		enemies;
	std::set<Enemy*>		removes;

	DirectX::XMFLOAT3 respawnPoint = { 0,5,0 };//エネミーの再出現位置
	int maxEnemies = 3; // エネミーの最大数
	
	std::map<Enemy*, float> delayRemoves;//遅延削除用のマップ

	float gomiTimer = 0.0f;

	// エネミーの反応処理用のマップ
	bool isAttracting = false;
	DirectX::XMFLOAT3 attractTarget;
	DirectX::XMFLOAT3 target;
	bool hasTarget = false;
	float attractCooldown = 0.0f;
};

