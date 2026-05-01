#include "UIManager.h"
#include "Player.h"
#include "SceneGame.h"

void UIManager::Init()
{
	uiElements.clear();// 一度クリア

	// ここでUIパーツの実体のみを作成する


}

// 全パーツの更新
void UIManager::Update(float elapsedTime)
{
	for (auto& ui : uiElements)
	{
		if (ui && ui->IsVisible())
		{
			// 各クラスがオーバーライドしたUpdateが呼ばれる
			ui->Update(elapsedTime);
		}
	}
}

// 全パーツの描画
void UIManager::Render(const RenderContext& rc)
{
	for (auto& ui : uiElements)
	{
		if (ui && ui->IsVisible())
		{
			ui->Render(rc);
		}
	}
}

// デバッグ用
void UIManager::DrawDebugGUI()
{
#ifdef _DEBUG
	// ImGuiで数値を変えたいときに!!

#endif
}
