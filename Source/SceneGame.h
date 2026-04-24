#pragma once

#include "Stage.h"
#include "Player.h"
#include "CameraController.h"
#include "Scene.h"
#include <gomi.h>
#include <Denti.h>
// ゲームシーン
//class SceneGame
class SceneGame : public Scene
{
public:
	SceneGame() {};
	//~SceneGame() {};
	~SceneGame() override {};
public:
	static SceneGame& Instance();
	void AddGomi(const DirectX::XMFLOAT3& pos);
	// 初期化
	//void Initialize();
	void Initialize()override;

	// 終了化
	//void Finalize();
	void Finalize()override;

	// 更新処理
	//void Update(float elapsedTime);
	void Update(float elapsedTime)override;

	// 描画処理
	//void Render();
	void Render()override;

	// GUI描画
	//void DrawGUI();
	void DrawGUI()override;


	
	float dentiSpawnTimer = 0.0f;
	float dentiSpawnInterval = 30.0f;
	int maxDenti = 10; 
private:
	// ?? 追加
	static SceneGame* instance;
	Stage* stage = nullptr;
	//Player* player = nullptr;
	CameraController* cameraController = nullptr;
	std::vector<Gomi*> gomis; // ゴミのリスト
	std::vector<Denti*> dentis;
	int gomiCount = 0;

	float timeLimit = 180.0f;   // 制限時間（秒）
	float currentTime = 180.0f; // 残り時間
	bool isTimeUp = false;
};
