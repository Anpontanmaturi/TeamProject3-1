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

	dentis.clear();

	dentis.clear();

	// 最初の1個
	Denti* d = new Denti();

	float x = GetRandom(-10.0f, 10.0f);
	float z = GetRandom(-10.0f, 10.0f);

	d->Init({ x, 0.5f, z });

	dentis.push_back(d);

	// タイマーリセット
	dentiSpawnTimer = 0.0f;

	//タイマー初期化
	currentTime = timeLimit;
	isTimeUp = false;

}

// 終了化
void SceneGame::Finalize()
{
	//エネミー終了化
	EnemyManager::Instance().Clear();

	//ステージ終了化
	if (stage != nullptr)
	{
		delete stage;
		stage = nullptr;
	}
	// ゴミ解放（重要）
	for (auto& g : gomis)
	{
		delete g;
	}
	gomis.clear();
	//プレイヤー終了化
	Player::Instance().Finalize();

	//カメラコントローラー終了化
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	if (isTimeUp) return;

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
	// Vキー押したら発動

	// Vキー

	static bool prev = false;
	bool now = (GetAsyncKeyState('V') & 0x8000) != 0;

	if (now && !prev)
	{
		DirectX::XMFLOAT3 playerPos = Player::Instance().GetPosition();
		EnemyManager::Instance().AttractEnemies(playerPos, 5.0f);


		//仮置き！！！！！
		gomiCount = 0;
	}

	prev = now;

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


	// =========================
	// カメラ更新（止めない）
	// =========================
	Camera::Instance().Update(elapsedTime);

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
	// 電池描画 ← これ追加
	for (auto& d : dentis)
	{
		d->Render(rc, modelRenderer);
	}
	// 3Dモデル描画
	{
		//ステージ描画
		stage->Render(rc, modelRenderer);

		//プレイヤー描画
		//player->Render(rc, modelRenderer);
		Player::Instance().Render(rc, modelRenderer);

		//エネミー描画
		EnemyManager::Instance().Render(rc, modelRenderer);
	}

	// 3Dデバッグ描画
	{
		//プレイヤーデバッグプリミティブ
		//player->RenderDebugPrimitive(rc, shapeRenderer);
		Player::Instance().RenderDebugPrimitive(rc, shapeRenderer);

		//エネミーデバッグプリミティブ描画
		EnemyManager::Instance().RenderDebugPrimitive(rc, shapeRenderer);
	}

	// 2Dスプライト描画
	{

	}
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

	//エネミーデバッグ描画
	//EnemyManager::Instance().DrawDebugGUI();

	ImGui::Begin("UI");
	ImGui::Text("Gomi : %d", gomiCount);
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