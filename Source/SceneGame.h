#pragma once

#include"Pause.h"
#include "Stage.h"
#include "Player.h"
#include "CameraController.h"
#include "Scene.h"
#include <System/AudioSource.h>
#include <gomi.h>
#include <Denti.h>
#include <garakuta.h>
#include <Pause.h>

#include"kagu.h"
#include <vector>
#include"kagu2.h"

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

	void StartHitStop(float time);

	void AddScore(int value);
	int GetScore() const { return score; }


	int GetGomiCount() const { return gomiCount; }
	void ClearGomiCount() { gomiCount = 0; }
	// コンボ
	void AddCombo();
	int GetCombo() const { return combo; }

	float GetComboMultiplier() const;

	// ゲッター
	float GetTimer() const { return currentTime; }

private:
	// ?? 追加
	static SceneGame* instance;
	Stage* stage = nullptr;
	//Player* player = nullptr;
	CameraController* cameraController = nullptr;
	std::vector<Gomi*> gomis; // ゴミのリスト
	std::vector<Denti*> dentis;
	int gomiCount = 0;
	std::vector<kagu*> kagus;
	std::vector<kagu2*> kagu2s;
	AudioSource* SGAu = nullptr;
	AudioSource* SGSe = nullptr;
	AudioSource* SGSe2 = nullptr;

#if 1
	float timeLimit = 180.0f;   // 制限時間（秒）
	float currentTime = 180.0f; // 残り時間
#else
	float timeLimit = 5.0f;   // 制限時間（秒）
	float currentTime = 5.0f; // 残り時間
#endif
	bool isTimeUp = false;
	float garbageSpawnTimer = 0.0f;
	float garbageSpawnInterval = 45.0f;
	int maxGarbage = 1;
	float timeScale = 1.0f;
	float hitStopTimer = 0.0f;
	bool isPaused = false;
	bool escKeyPrev = false; // トグル用
	// コンボ
	int combo = 0;
	float comboTimer = 0.0f;
	float comboLimit = 2.0f; // 2秒以内で継続
	Pause pause;

	//std::vector<kagu*> kagus;

	// ガラクタスポーンタイマー
	std::vector<garakuta*> garbages;


public:
	int score = 0;

};
