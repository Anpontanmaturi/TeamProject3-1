#include"Player.h"
#include"System/Input.h"
#include <imgui.h>
#include "Camera.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include "ProjectileHoming.h"
#include <SceneGame.h>
#include <System/Audio.h>

//初期化
void Player::Initialize()
{
	model = new Model("Data/Model/Player/runba_on_tex.mdl");

	//モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.025f;

	energy = 1000.0f;

	position = { 0.0f,0.0f,0.0f };

	stepOffset = 0.02f;

	Reset();

	PSE1 = Audio::Instance().LoadAudioSource("Data/Sound/runba.wav");
	PSE2 = Audio::Instance().LoadAudioSource("Data/Sound/tobasu.wav");
	PSE3 = Audio::Instance().LoadAudioSource("Data/Sound/seti.wav");
}

void Player::AddEnergy(float value)
{
	energy += value;

	// 上限つけたいなら（おすすめ）
	if (energy > maxenergy)
	{
		energy = maxenergy;
	}
}

//終了化
void Player::Finalize()
{
	PSE1->Stop();
	PSE2->Stop();
	PSE3->Stop();
	delete model;
}


//更新処理
void Player::Update(float elapsedTime)
{

	if (!canMove)
	{
		// 横方向速度停止
		velocity.x = 0.0f;
		velocity.z = 0.0f;

		// Transform更新は必要
		UpdateTransform();
		model->UpdateTransform();

		return;
	}


	//移動入力処理
	InputMove(elapsedTime);

	//ジャンプ入力処理
	InputJump();

	//弾丸入力処理
	InputProjectile();

	//速力処理更新
	UpdateVelocity(elapsedTime);

	//弾丸更新処理
	projectileManager.Update(elapsedTime);

	//プレイヤーとエネミーとの衝突処理
	CollisionPlayerVsEnemies();

	//弾丸と敵の衝突処理
	CollisionProjectilesVsEnemies();

	//オブジェクト行列を更新
	UpdateTransform();

	//モデル行列更新
	model->UpdateTransform();

}

void Player::AddGarbage(int value)
{
	garbageCount += value;
}
void Player::Reset()
{
	//========================
	// 基本ステータス
	//========================

	garbageCount = 0;

	energy = maxenergy;

	hp = maxHp;

	//========================
	// 座標
	//========================

	position = { 0.0f,0.0f,0.0f };

	angle = { 0.0f,0.0f,0.0f };

	scale = { 0.025f,0.025f,0.025f };

	//========================
	// 速度
	//========================

	velocity = { 0.0f,0.0f,0.0f };

	//========================
	// ジャンプ
	//========================

	jumpCount = 0;

	//========================
	// ブースト
	//========================

	isBoost = false;

	//========================
	// 操作可能状態
	//========================

	canMove = true;

	//========================
	// Transform更新
	//========================

	UpdateTransform();

	if (model != nullptr)
	{
		model->UpdateTransform();
	}
}


//着地した時に呼ばれる
void Player::OnLanding()
{
	jumpCount = 0;
}

//移動入力処理
void Player::InputMove(float elapsedTime)
{
	//進行ベクトル取得
	DirectX::XMFLOAT3 moveVec = GetMoveVec();

	//移動処理
	Move(elapsedTime, moveVec.x, moveVec.z, moveSpeed);

	//旋回処理
	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
	//追加
	moveSpeed = moveLimit;
	if (energy <= 0.0f)
	{
		moveSpeed = moveSpeed / 2;

		// エネルギー切れなら音停止
		PSE1->Stop();
	}
	isBoost = false; // ←毎フレーム初期化

	moveSpeed = moveLimit;

	if (energy <= 0.0f)
	{
		moveSpeed *= 0.5f;
	}
	else
	{
		energy -= 0.05f;

		
		bool boostKey = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;

		if (boostKey)
		{
			isBoost = true;
			moveSpeed = boostLimit;
			energy -= 1.0f;

			// まだ再生してない時だけ再生
			static bool isPlaying = false;

			
				PSE1->Play(true);
			
		}
		else
		{
			// スペース離したら停止
			PSE1->Stop();

			
		}
		
		
	
	}
}

//描画処理
void Player::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, model, ShaderId::Lambert);

	//弾丸描画処理
	projectileManager.Render(rc, renderer);
}

//デバッグプリミティブ描画
void Player::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	//基底クラスの呼び出し
	Character::RenderDebugPrimitive(rc, renderer);

	//弾丸デバッグプリミティブ描画
	projectileManager.RenderDebugPrimitive(rc, renderer);
}

// デバッグ用GUI描画
void Player::DrowDebugGUI()
{
	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y + 10), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
	{
		//トランスフォーム
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//位置
			ImGui::InputFloat3("Position", &position.x);
			//回転
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(angle.x);
			a.y = DirectX::XMConvertToDegrees(angle.y);
			a.z = DirectX::XMConvertToDegrees(angle.z);
			ImGui::InputFloat3("Angle", &a.x);
			angle.x = DirectX::XMConvertToRadians(a.x);
			angle.y = DirectX::XMConvertToRadians(a.y);
			angle.z = DirectX::XMConvertToRadians(a.z);
			//スケール
			ImGui::InputFloat3("Scale", &scale.x);
		//追加
			ImGui::InputFloat("energy", &energy);
			ImGui::Text("Press TAB to toggle the cursor move and stop.");
		}
	}
	ImGui::End();
}

//スティック入力値から移動ベクトルを取得
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
	//入力情報を取得
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();

	//カメラ方向とスティックの入力値によって進行方向を計算する
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	//移動ベクトルはXZ平面に水平なベクトルになるようにする

	//カメラ右方向ベクトルをXZ単位ベクトルに変換
	float cameraRightX = cameraRight.x;
	float cameraRightZ = cameraRight.z;
	float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
	if (cameraRightLength > 0.0f)
	{
		//単位ベクトル化
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}

	//カメラ前方向ベクトルをXZベクトルに変換
	float cameraFrontX = cameraFront.x;
	float cameraFrontZ = cameraFront.z;
	float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
	if (cameraFrontLength > 0.0f)
	{
		//単位ベクトル化
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;		
	}
	//スティックの水平入力値をカメラ右方向に反映し、
	//スティックの垂直入力値をカメラ前方向に反映し、
	//進行ベクトルを計算する
	DirectX::XMFLOAT3 vec;
	vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
	vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);
	//Y軸方向には移動しない
	vec.y = 0.0f;

	return vec;
}

//プレイヤーとエネミーとの衝突処理
void Player::CollisionPlayerVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	//全ての敵と総当たりで衝突処理
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);

		//衝突処理
		DirectX::XMFLOAT3 outPosition;
		if (Collision::IntersectCylinderVsCylinder(
			position,
			radius,
			height,
			enemy->GetPosition(),
			enemy->GetRadius(),
			enemy->GetHeight(),
			outPosition
		))
		{
			//敵の真上に当たったかを判定
			DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
			DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
			DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
			DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
			DirectX::XMFLOAT3 normal;
			DirectX::XMStoreFloat3(&normal, N);
			//上から踏んづけた場合は小ジャンプする
			if (normal.y > 0.8f)
			{
				//小ジャンプする
				Jump(jumpSpeed * 0.5f);
			}
			else
			{
				if (isBoost)
				{

					if (!enemy->ApplyDamage(1, 0.3f))
						continue;

					// ===== ノックバック処理 =====
					DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
					V = DirectX::XMVector3Normalize(V);

					DirectX::XMFLOAT3 dir;
					DirectX::XMStoreFloat3(&dir, V);

					DirectX::XMFLOAT3 impulse;
					float power = 12.5f;

					impulse.x = dir.x * power;
					impulse.y = power * 1.5f;
					impulse.z = dir.z * power;

					// 既にあるこれ使うのがベスト
					enemy->AddImpulse(impulse);

					// x秒後に敵を削除する
					enemyManager.RemoveWithDelay(enemy, deleteEnemyTimer);

					Camera::Instance().StartShake(0.9f, 0.4f);
					SceneGame::Instance().StartHitStop(0.28f);
					energy += 50.0f;


					// コンボ追加
					SceneGame::Instance().AddCombo();
					int combo = SceneGame::Instance().GetCombo();
					float multiplier = SceneGame::Instance().GetComboMultiplier();
					int gomi = SceneGame::Instance().GetGomiCount();
					SceneGame::Instance().AddScore((100 + (gomi * 10)) * multiplier);	

					PSE2->Play(false);
					
				}
				
				else
				{
					//通常
					enemy->SetPosition(outPosition);
					
				}
			}
		}
	}
}

void Player::Heal(float amount)
{
	hp += amount;
	if (hp > maxHp) hp = maxHp;
}


//ジャンプ入力処理
void Player::InputJump()
{
	
}

//入力処理
void Player::InputProjectile()
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	

	static bool prevE = false;
	bool nowE = (GetAsyncKeyState('E') & 0x8000) != 0;

	if (nowE && !prevE)
	{
		OutputDebugStringA("E押された\n");

		if (garbageCount > 0)
		{
			OutputDebugStringA("ガラクタ使用\n");

			garbageCount--;

			DirectX::XMFLOAT3 pos = position;
			EnemyManager::Instance().AttractEnemies(pos, 10.0f);
			PSE3->Play(false);
		}
	}
	prevE = nowE;

}

//弾丸と敵の衝突処理
void Player::CollisionProjectilesVsEnemies()
{
}

