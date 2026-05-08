#include "System/Graphics.h"
#include "SceneGame.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "Player.h"
#include"gomi.h"
#include <cstdlib>
#include <Windows.h> 
#include <imgui.h>
#include <algorithm> 
#include <cmath>
#include<SceneResult.h>
#include <SceneManager.h>
#include <ctime>
#include <Denti.h>
#include"SceneTitle.h"
#include <Pause.h>
#include"kagu.h"
#include"Pause.h"
std::vector<Object> objects;
#include <Denti.h>
#include "UiManager.h"



// 距離計算
float GetDistance(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - b.z;

	return sqrtf(dx * dx + dy * dy + dz * dz);
}



float GetRandom(float min, float max)
{
	return min + (float)rand() / RAND_MAX * (max - min);
}
// 初期化
void SceneGame::Initialize()
{
	srand((unsigned int)time(nullptr));
	//ステージ初期化
	stage = new Stage();
	instance = this;

	//プレイヤー初期化
	Player::Instance().Initialize();
	pause.Initialize();

	pause.Initialize();
	//プレイヤー初期化
	Player::Instance().Initialize();

	// UIの初期化
	UIManager::Instance().Initialize();

	//カメラコントローラー初期化
	cameraController = new CameraController();
	
	//カメラ初期設定
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),	//視点
		DirectX::XMFLOAT3(0, 0, 0),		//注視点
		DirectX::XMFLOAT3(0, 1, 0)		//上方向
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),	//視野角
		graphics.GetScreenWidth()/graphics.GetScreenHeight(),	//画面アスペクト比
		0.1f,	//グリップ距離(近)
		1000.0f	//グリップ距離(遠)
	);

	//エネミー初期化
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < ENEMY_MAX; ++i)
	{
		EnemySlime* slime = new EnemySlime();
		slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));

		slime->SetTerritory(slime->GetPosition(), 15.0f);

		enemyManager.Register(slime);
	}
	// =========================
   // ゴミ生成（ここが本命）
   // =========================
	gomis.clear();

	for (int i = 0; i < 20; i++)
	{
		Gomi* g = new Gomi(); // ← ポインタで生成

		float x = GetRandom(-10.0f, 10.0f);
		float z = GetRandom(-10.0f, 10.0f);

		g->Init({ x, 0.0f, z });

		gomis.push_back(g);
	}

	// オブジェクト
	// オブジェクト生成

	objects.emplace_back(); // ← これが必要！！

	// 位置など設定
	objects[0].SetPosition(0, 0, 0);

	garbages.clear();

	dentis.clear();

	
	garbages.clear(); // ←これ追加！！

	// 電池1個
	Denti* d = new Denti();
	float x = GetRandom(-10.0f, 10.0f);
	float z = GetRandom(-10.0f, 10.0f);
	d->Init({ x, 0.5f, z });
	dentis.push_back(d);

	// =========================
// 冷蔵庫生成
// =========================
	kagus.clear();

	kagu* k = new kagu();
	k->Init({ 3.0f, 0.0f, 3.0f });

	kagus.push_back(k);
	// ガラクタ生成
	garbages.clear();

	// 最初の1個
	garakuta* g = new garakuta();

	// ガラクタ専用の座標を作る
	float gx = GetRandom(-10.0f, 10.0f);
	float gz = GetRandom(-10.0f, 10.0f);

	g->Init({ gx, 0.0f, gz });
	garbages.push_back(g);

	// タイマーリセット
	garbageSpawnTimer = 0.0f;

	// 最初の1個

	//dentis.push_back(d);
	// ガラクタ生成
	garbages.clear();


	// 最初の1個
	garakuta* g = new garakuta();

	// ガラクタ専用の座標を作る
	float gx = GetRandom(-10.0f, 10.0f);
	float gz = GetRandom(-10.0f, 10.0f);

	g->Init({ gx, 0.0f, gz });
	garbages.push_back(g);

	// タイマーリセット
	garbageSpawnTimer = 0.0f;
	// タイマーリセット
	dentiSpawnTimer = 0.0f;

	//タイマー初期化
	currentTime = timeLimit;
	isTimeUp = false;

}

// 終了化
void SceneGame::Finalize()
{
	EnemyManager::Instance().Clear();

	if (stage != nullptr)
	{
		delete stage;
		stage = nullptr;
	}

	for (auto& g : gomis) delete g;
	gomis.clear();
	for (auto& k : kagus)
	{
		delete k;
	}

	kagus.clear();

	dentis.clear();

	//オブジェクト終了化
	objects.clear();
	for (auto& g : garbages) delete g;
	garbages.clear();
	pause.Finalize();
	//プレイヤー終了化

	Player::Instance().Finalize();

	for (auto& g : garbages) delete g;
	garbages.clear();

	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	pause.Finalize();
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{



	pause.Update();
	//ポーズ中ならゲームを止める
	if (pause.IsPaused())
	{
		return;
	}
	if (isTimeUp) return;



	// ポーズ更新
	pause.Update();

	// ポーズ中ならゲーム止める
	if (pause.IsPaused())
	{
		return;
	}
	// =========================
	// ヒットストップ処理（追加）
	// =========================
	if (hitStopTimer > 0.0f)
	{
		hitStopTimer -= elapsedTime;

		if (hitStopTimer <= 0.0f)
		{
			timeScale = 1.0f;
		}
	}

	float scaledTime = elapsedTime * timeScale;

	// =========================
	// タイマー（※これは止めない）
	// =========================
	currentTime -= elapsedTime;

	if (currentTime <= 0.0f)
	{
		currentTime = 0.0f;
		isTimeUp = true;
		SceneManager::Instance().ChangeScene(new SceneResult);
		return;
	}

	// 電池スポーンタイマー
	dentiSpawnTimer += elapsedTime;

	if (dentiSpawnTimer >= dentiSpawnInterval)
	{
		dentiSpawnTimer = 0.0f;

		if (dentis.size() < maxDenti)
		{
			Denti* d = new Denti();

			float x = GetRandom(-10.0f, 10.0f);
			float z = GetRandom(-10.0f, 10.0f);

			d->Init({ x, 0.5f, z });

			dentis.push_back(d);
		}
	}
	//カメラコントローラー更新処理
	//DirectX::XMFLOAT3 target = player->GetPosition();

	// =========================
	// カメラ
	// =========================

	DirectX::XMFLOAT3 target = Player::Instance().GetPosition();
	target.y += 0.5f;

	cameraController->SetTarget(target);
	cameraController->Update(scaledTime);

	// =========================
	// ゲーム更新（全部 scaledTime）
	// =========================
	stage->Update(scaledTime);
	Player::Instance().Update(scaledTime);
	EnemyManager::Instance().Update(scaledTime);


	// UIの更新
	UIManager::Instance().Update(scaledTime);


	//オブジェクト更新処理
	for (auto& obj : objects)
	{
		obj.Update(elapsedTime);

		// プレイヤー位置
		DirectX::XMFLOAT3 playerPos = Player::Instance().GetPosition();

		// オブジェクト位置
		DirectX::XMFLOAT3 objPos = obj.GetPosition();

		// 距離
		float distance = GetDistance(playerPos, objPos);

		float radius = 1.0f;

		if (distance < radius)
		{
			// ★ここが回復速度
			Player::Instance().AddEnergy(85.0f * elapsedTime);
			gomiCount = 0;
		}
	}

	//プレイヤー更新処理
	//player->Update(elapsedTime);
	//Player::Instance().Update(elapsedTime);


	// =========================
	// ゴミ処理
	// =========================
	DirectX::XMFLOAT3 playerPos = Player::Instance().GetPosition();

	for (auto& g : gomis)
	{
		g->Update(scaledTime);

		float dist = GetDistance(playerPos, g->GetPosition());
		float playerRadius = 0.7f;

		if (!g->IsCollected() && dist < playerRadius + g->GetRadius())
		{
			g->Collect();
			gomiCount++;
			score += 5;
		}
	}

	// =========================
// ガラクタ処理（←これ追加）
// =========================


	// =========================
	// ガラクタ処理（←これ追加）
	// =========================

	for (auto& g : garbages)
	{
		g->Update(elapsedTime);

		float dist = GetDistance(playerPos, g->GetPosition());
		float playerRadius = 0.7f;

		if (!g->IsCollected() && dist < playerRadius + g->GetRadius())
		{
			g->Collect();

			Player::Instance().AddGarbage(1); // ← 持ってる数増やす
		}
	}
	// =========================
	// ガラクタスポーン
	// =========================
	garbageSpawnTimer += elapsedTime;

	if (garbageSpawnTimer >= garbageSpawnInterval)
	{
		garbageSpawnTimer = 0.0f;

		// 最大数チェック（1個だけ）
		if (garbages.size() < maxGarbage)
		{
			garakuta* g = new garakuta();

			float x = GetRandom(-5.0f, 5.0f);
			float z = GetRandom(-5.0f, 5.0f);

			g->Init({ x, 0.0f, z });

			garbages.push_back(g);
		}
	}




	for (auto& d : dentis)
	{
		d->Update(elapsedTime);


		float dist = GetDistance(playerPos, d->GetPosition());

		float playerRadius = 0.7f;

		if (!d->IsCollected() && dist < playerRadius + d->GetRadius())
		{
			d->Collect();

			
			Player::Instance().AddEnergy(300.0f);
		}
	}
	
	for (auto& k : kagus)
	{
		k->Update(elapsedTime);

		DirectX::XMFLOAT3 p = Player::Instance().GetPosition();
		DirectX::XMFLOAT3 kp = k->GetPosition();

		// =========================
		// 冷蔵庫の四角い当たり判定
		// =========================

		float halfWidth = 1.6f; // 横幅
		float halfDepth = 1.6f; // 奥行き

		float minX = kp.x - halfWidth;
		float maxX = kp.x + halfWidth;

		float minZ = kp.z - halfDepth;
		float maxZ = kp.z + halfDepth;

		// プレイヤーが箱の中にいるか
		bool hit =
			(p.x > minX && p.x < maxX) &&
			(p.z > minZ && p.z < maxZ);

		if (!k->IsBroken() && hit)
		{
			// =========================
			// ダッシュ中なら破壊
			// =========================
			if (Player::Instance().IsBoost())
			{
				k->Break();
			}
			else
			{
				// =========================
				// 押し戻し
				// =========================

				float left = abs(p.x - minX);
				float right = abs(maxX - p.x);
				float top = abs(maxZ - p.z);
				float bottom = abs(p.z - minZ);

				float minDist = left;
				int dir = 0;

				if (right < minDist)
				{
					minDist = right;
					dir = 1;
				}

				if (top < minDist)
				{
					minDist = top;
					dir = 2;
				}

				if (bottom < minDist)
				{
					minDist = bottom;
					dir = 3;
				}

				float push = 0.05f;

				switch (dir)
				{
				case 0: // 左
					p.x = minX - push;
					break;

				case 1: // 右
					p.x = maxX + push;
					break;

				case 2: // 上
					p.z = maxZ + push;
					break;

				case 3: // 下
					p.z = minZ - push;
					break;
				}

				Player::Instance().SetPosition(p);
			}
		}
	}




	// =========================
	// ゴミ削除
	// =========================
	gomis.erase(
		std::remove_if(gomis.begin(), gomis.end(),
			[](Gomi* g)
			{
				if (g->IsCollected())
				{
					delete g;
					return true;
				}
				return false;
			}),
		gomis.end()
	);

	dentis.erase(
		std::remove_if(dentis.begin(), dentis.end(),
			[](Denti* d)
			{
				if (d->IsCollected())
				{
					delete d;
					return true;
				}
				return false;
			}),
		dentis.end()
	);



	// ガラクタ削除

	garbages.erase(
		std::remove_if(garbages.begin(), garbages.end(),
			[](garakuta* g)
			{
				if (g->IsCollected())
				{
					delete g;
					return true;
				}
				return false;
			}),
		garbages.end()
	);





	// =========================
	// カメラ更新（止めない）
	// =========================
	Camera::Instance().Update(elapsedTime);


	// コンボ管理
	if (combo > 0)
	{
		comboTimer -= elapsedTime;

		if (comboTimer <= 0.0f)
		{
			combo = 0;
		}
	}
}


// 描画処理
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ShapeRenderer* shapeRenderer = graphics.GetShapeRenderer();
	ModelRenderer* modelRenderer = graphics.GetModelRenderer();

	//描画処理
	RenderContext rc;
	rc.deviceContext = dc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f };	// ライト方向（下方向）

	// 描画準備
	
	rc.renderState = graphics.GetRenderState();

	//カメラパラメータ設定
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// ゴミ描画
	for (auto& g : gomis)
	{
		g->Render(rc, modelRenderer);
	}
	for (auto& g : garbages)
	{
		g->Render(rc, modelRenderer);
	}
	// ガラクタ描画
	for (auto& g : garbages)
	{
		g->Render(rc, modelRenderer);
	}
	// 電池描画 ← これ追加
	for (auto& d : dentis)
	{
		d->Render(rc, modelRenderer);
	}
	// 冷蔵庫描画
	for (auto& k : kagus)
	{
		k->Render(rc, modelRenderer);
	}
	// 3Dモデル描画
	{
		//ステージ描画
		stage->Render(rc, modelRenderer);

		//プレイヤー描画
		Player::Instance().Render(rc, modelRenderer);

		//エネミー描画
		EnemyManager::Instance().Render(rc, modelRenderer);
	}

	// 3Dデバッグ描画
	{
		//プレイヤーデバッグプリミティブ
		Player::Instance().RenderDebugPrimitive(rc, shapeRenderer);

		//エネミーデバッグプリミティブ描画
		EnemyManager::Instance().RenderDebugPrimitive(rc, shapeRenderer);
	}

	// 2Dスプライト描画
	{


		//UI描画
		UIManager::Instance().Render(rc);
	}

	
	
	// g最後にポーズ描画（上にかぶせる）
	pause.Render();
	

	pause.Render();

}
SceneGame* SceneGame::instance = nullptr;

SceneGame& SceneGame::Instance()
{
	return *instance;
}

void SceneGame::AddGomi(const DirectX::XMFLOAT3& pos)
{
	// 最大数チェック
	const int MAX_GOMI = 5;

	if (gomis.size() >= MAX_GOMI)
	{
		// 一番古いゴミを削除
		delete gomis.front();
		gomis.erase(gomis.begin());
	}

	// 新しいゴミ追加
	Gomi* g = new Gomi();
	g->Init(pos);
	gomis.push_back(g);
}
// GUI描画
void SceneGame::DrawGUI()
{
	//プレイヤーデバッグ描画
	Player::Instance().DrowDebugGUI();

	// UI用デバッグGUI描画
	UIManager::Instance().DrawDebugGUI();

	//エネミーデバッグ描画
	//EnemyManager::Instance().DrawDebugGUI();

	ImGui::Begin("UI");
	ImGui::Text("Gomi : %d", gomiCount);
	ImGui::Separator();
	if (ImGui::Button("Spawn Garbage (Player Front)"))
	{
		// プレイヤーの前にスポーンさせる
		DirectX::XMFLOAT3 pos = Player::Instance().GetPosition();
		pos.x += 2.0f;
		garakuta* g = new garakuta();
		g->Init(pos);
		garbages.push_back(g);
	}
	ImGui::End();


	ImGui::Begin("Time");

	if (isTimeUp)
	{
		ImGui::Text("Time Up!");
	}
	else
	{
		ImGui::Text("Time : %.1f", currentTime);
	}

	ImGui::End();


	ImGui::Begin("Score");
	ImGui::Text("Score : %d", score);
	ImGui::Text("Combo : %d", SceneGame::Instance().GetCombo());
	ImGui::End();
}

void SceneGame::StartHitStop(float time)
{
	hitStopTimer = time;
	timeScale = 0.0f;
}


void SceneGame::AddScore(int value)
{
	score += value;
}

void SceneGame::AddCombo()
{
	combo++;
	comboTimer = comboLimit;
}

float SceneGame::GetComboMultiplier() const
{
	if (combo >= 3) return 3.0f;
	if (combo == 2) return 1.5f;
	return 1.0f;
}