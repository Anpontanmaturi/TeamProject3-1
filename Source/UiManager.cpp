#include "UIManager.h"
#include "Player.h"
#include "SceneGame.h"
#include "UiBattery.h"
#include <imgui.h>

void UIManager::Initialize()
{
	uiElements.clear();// 一度クリア

	// ここでUIパーツの実体のみを作成する
	uiElements.push_back(std::make_unique<UIBattery>());

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
	ImGui::Begin("UI Manager Debug");

	//ImGui::SliderFloat("Battery Adjust X", &static_cast<UIBattery*>(uiElements[0].get())->adjX, -20.0f, 20.0f); //batteryの微調整用スライダー

	// 位置調整用
	{
		UIBattery* battery = static_cast<UIBattery*>(uiElements[0].get());
		DirectX::XMFLOAT2 pos = battery->GetPosition();
		float posArr[2] = { pos.x, pos.y };
		if (ImGui::DragFloat2("BatteryUI Position", posArr, 1.0f)) {
			battery->SetPosition(posArr[0], posArr[1]);
		}
	}

	ImGui::End();
#endif
}
