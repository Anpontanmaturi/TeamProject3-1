#include "SceneManager.h"

//更新処理
void SceneManager::Update(float elapsedTime)
{
	if (nextScene != nullptr)
	{
		// 古いシーン削除
		Clear();

		// 新しいシーン
		currentScene = nextScene;
		nextScene = nullptr;

		//必ず初期化
		currentScene->Initialize();
	}

	if (currentScene != nullptr)
	{
		currentScene->Update(elapsedTime);
	}
}

//描画処理
void SceneManager::Render()
{
	if (currentScene != nullptr)
	{
		currentScene->Render();
	}
}


//GUI描画
void SceneManager::DrawGUI()
{
	if (currentScene != nullptr)
	{
		currentScene->DrawGUI();
	}
}

//シーンクリア
void SceneManager::Clear()
{
	if (currentScene != nullptr)
	{
		currentScene->Finalize();
		delete currentScene;
		currentScene = nullptr;
	}
}

//シーン切り替え
void SceneManager::ChangeScene(Scene* scene)
{
	//新しいシーンを設定
	nextScene = scene;
}