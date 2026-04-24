#pragma once

#include "Stage.h"
#include "Player.h"
#include "CameraController.h"
#include "Scene.h"
#include <gomi.h>

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


	void StartHitStop(float time);

	void AddScore(int value);
	int GetScore() const { return score; }


	int GetGomiCount() const { return gomiCount; }
	void ClearGomiCount() { gomiCount = 0; }
private:
	// ?? 追加
	static SceneGame* instance;
	Stage* stage = nullptr;
	//Player* player = nullptr;
	CameraController* cameraController = nullptr;
	std::vector<Gomi*> gomis; // ゴミのリスト

	int gomiCount = 0;

	float timeLimit = 180.0f;   // 制限時間（秒）
	float currentTime = 180.0f; // 残り時間
	bool isTimeUp = false;

	float timeScale = 1.0f;
	float hitStopTimer = 0.0f;

public:
	int score = 0;

};
