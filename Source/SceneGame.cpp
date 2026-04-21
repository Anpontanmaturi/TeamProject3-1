#include "System/Graphics.h"
#include "SceneGame.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "Player.h"
#include"gomi.h"
#include <cstdlib>
#include <ctime>
#include <Windows.h> 
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
	//player = new Player();
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
	/*EnemySlime* slime = new EnemySlime();
	slime->SetPosition(DirectX::XMFLOAT3(0, 0, 5));
	enemyManager.Register(slime);*/
	for (int i = 0; i < 2; ++i)
	{
		EnemySlime* slime = new EnemySlime();
		slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));

		slime->SetTerritory(slime->GetPosition(), 10.0f);

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
	/*if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}*/
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
	//カメラコントローラー更新処理
	//DirectX::XMFLOAT3 target = player->GetPosition();
	DirectX::XMFLOAT3 target = Player::Instance().GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	//ステージ更新処理
	stage->Update(elapsedTime);

	//プレイヤー更新処理
	//player->Update(elapsedTime);
	Player::Instance().Update(elapsedTime);

	//エネミー更新処理
	EnemyManager::Instance().Update(elapsedTime);
	// ゴミ更新処理
	for (auto& g : gomis)
	{
		g->Update(elapsedTime);
	}
	// Vキー押したら発動
	static bool prev = false;
	bool now = (GetAsyncKeyState('V') & 0x8000) != 0;

	if (now && !prev)
	{
		DirectX::XMFLOAT3 playerPos = Player::Instance().GetPosition();
		EnemyManager::Instance().AttractEnemies(playerPos, 5.0f);
	}

	prev = now;
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
	//player->DrowDebugGUI();
	Player::Instance().DrowDebugGUI();
}
