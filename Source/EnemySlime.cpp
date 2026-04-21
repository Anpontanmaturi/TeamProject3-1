#include "EnemySlime.h"
#include "MathUtils.h"
#include "Player.h"
#include "ProjectileStraight.h"

//コンストラクタ
EnemySlime::EnemySlime()
{
	model = new Model("Data/Model/Slime/Slime.mdl");

	//モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.01f;

	//幅、大きさ設定
	radius = 0.5f;
	height = 1.0f;

	//徘徊ステートへ遷移
	SetWanderState();
}

EnemySlime::~EnemySlime()
{
	delete model;
}

//更新処理
void EnemySlime::Update(float elapsedTime)
{
	Enemy::Update(elapsedTime);
	//ステート毎の更新処理
	switch (state)
	{
	case State::Wander:
		UpdateWanderState(elapsedTime);
		break;
		
	case State::Idle:
		UpdateIdleState(elapsedTime);
		break;

	case State::Escepe:
		UpdateEscepeState(elapsedTime);
		break;
	}

	//速力処理更新
	UpdateVelocity(elapsedTime);

	//弾丸更新処理
	projectileManager.Update(elapsedTime);

	//無敵時間更新
	UpdateInvincibleTimer(elapsedTime);

	//オブジェクト行列を更新
	UpdateTransform();

	//モデル行列更新
	model->UpdateTransform();
}

//描画処理
void EnemySlime::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, model, ShaderId::Lambert);

	//弾丸描画処理
	projectileManager.Render(rc, renderer);
}

//デバッグプリミティブ描画
void EnemySlime::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	//基底クラスのデバッグプリミティブ描画
	Enemy::RenderDebugPrimitive(rc, renderer);

	//縄張り範囲をデバッグ円柱描画
	renderer->RenderCylinder(rc, territoryOrigin, territoryRange,
								1.0f, DirectX::XMFLOAT4(0, 1, 0, 1));

	//ターゲット位置をデバッグ球描画
	renderer->RenderSphere(rc, targetPosition, 1.0f, DirectX::XMFLOAT4(1, 1, 0, 1));

	//作的範囲をデバッグ円柱描画
	renderer->RenderCylinder(rc, position, searchRange, 1.0f, DirectX::XMFLOAT4(1, 0, 0, 1));
}

//縄張り設定
void EnemySlime::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
	territoryOrigin = origin;
	territoryRange = range;
}

//ターゲット位置をランダム設定
void EnemySlime::SetRandomTargetPosition()
{
	float theta = MathUtils::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = MathUtils::RandomRange(0.0f, territoryRange);
	targetPosition.x = territoryOrigin.x + sinf(theta) * range;
	targetPosition.y = territoryOrigin.y;
	targetPosition.z = territoryOrigin.z + cosf(theta) * range;
}

//目標地点へ移動
void EnemySlime::MoveToTarget(float elapsedTime, float moveSpeedRate, float turnSpeedRate)
{
	//ターゲット方向への進行ベクトルを算出
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;

	//移動処理
	Move(elapsedTime, vx, vz, moveSpeed * moveSpeedRate);
	Turn(elapsedTime, vx, vz, turnSpeed * turnSpeedRate);
}

//徘徊ステートへ遷移
void EnemySlime::SetWanderState()
{
	state = State::Wander;

	//目的地設定
	SetRandomTargetPosition();
}

//プレイヤー索敵
bool EnemySlime::SearchPlayer()
{
	//プレイヤーとの高低差を利用して3Dでの距離判定をする
	const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
	float vx = playerPosition.x - position.x;
	float vy = playerPosition.y - position.y;
	float vz = playerPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	if (dist < searchRange)
	{
		// 発見
		return true;
	}
	return false;
}

//徘徊ステート更新処理
void EnemySlime::UpdateWanderState(float elapsedTime)
{
	//目標地点までXZ平面での距離判定
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float distSq = vx * vx + vz * vz;
	if (distSq < radius * radius)
	{
		/*//次の目的地設定
		SetRandomTargetPosition();*/
		//待機ステートへ遷移
		SetIdleState();
	}

	//目標地点へ移動
	MoveToTarget(elapsedTime, 1.0f, 1.0f);

	//プレイヤー索敵
	if (SearchPlayer())
	{
		//見つかったら逃走ステートへ遷移
		SetEscepeState();
	}
}

//待機ステートへ遷移
void EnemySlime::SetIdleState()
{
	state = State::Idle;

	// 動く速度を元に戻す
	moveSpeed = DEFAULT_SPEED;

	//タイマーをランダム設定
	stateTimer = 0.1f;

}

//待機ステート更新処理
void EnemySlime::UpdateIdleState(float elapsedTime)
{
	//タイマー処理
	stateTimer -= elapsedTime;
	if (stateTimer < 0.0f)
	{
		//徘徊ステートへ遷移
		SetWanderState();
	}

	//プレイヤー索敵
	if (SearchPlayer())
	{
		//見つかったら逃走ステートへ遷移
		SetEscepeState();
	}
}

//逃走ステートへ遷移
void EnemySlime::SetEscepeState()
{
	state = State::Escepe;

	// 動く速度を上げる
	moveSpeed = DEFAULT_SPEED * 3.0f;

	stateTimer = 0.0f;
}

//逃走ステート更新処理
void EnemySlime::UpdateEscepeState(float elapsedTime)
{
	//プレイヤーから自分の位置へのベクトルを算出
	DirectX::XMVECTOR playerPos = DirectX::XMLoadFloat3(&Player::Instance().GetPosition());
	DirectX::XMVECTOR myPos = DirectX::XMLoadFloat3(&position);

	//プレイヤーを見失っていない
	if (SearchPlayer())
	{
		// 逃げる方向を計算
		DirectX::XMVECTOR dir = DirectX::XMVectorSubtract(myPos, playerPos);

		//Y軸成分を0にして正規化する
		dir = DirectX::XMVectorSetY(dir, 0.0f);
		DirectX::XMVECTOR escapeDir = DirectX::XMVector3Normalize(dir);

		//目標地点を算出
		DirectX::XMVECTOR targetVec = DirectX::XMVectorAdd(myPos, DirectX::XMVectorScale(escapeDir, 10.0f));
		DirectX::XMStoreFloat3(&targetPosition, targetVec);
	}
	// プレイヤーを見失った
	else
	{
		// 目標地点まで XZ 平面でどれくらい近いか判定
		float vx = targetPosition.x - position.x;
		float vz = targetPosition.z - position.z;
		float distSq = vx * vx + vz * vz;

		// 到着したら待機へ
		if (distSq < radius * radius)
		{
			SetIdleState();
			return;
		}
	}

	//目標地点へ移動
	MoveToTarget(elapsedTime, 1.0f, 1.0f);
}