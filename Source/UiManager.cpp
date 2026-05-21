#include "UIManager.h"
#include "Player.h"
#include "SceneGame.h"
#include <imgui.h>
#include "UiBattery.h"
#include "UiDustCount.h"
#include "UiTimer.h"
#include "UiMyScore.h"
#include "UiHighScore.h"
#include "UiGabageIcon.h"
#include "UiComboCounter.h"

enum
{
	BATTERY = 0,
	DUSTCOUNT,
	TIMER,
	HIGHSCORE,
	MYSCORE,
	GARBAGEICON,
	COMBOCOUNTER,
};

void UIManager::Initialize()
{
	uiElements.clear();// 一度クリア

	// ここでUIパーツの実体のみを作成する
	uiElements.push_back(std::make_unique<UIBattery>());
	uiElements.push_back(std::make_unique<UIDustCount>());
	uiElements.push_back(std::make_unique<UiTimer>());
	uiElements.push_back(std::make_unique<UiHighScore>());
	uiElements.push_back(std::make_unique<UiMyScore>());
	uiElements.push_back(std::make_unique<UiGarbageIcon>());
	uiElements.push_back(std::make_unique<UIComboCounter>());

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
		UIBattery* battery = static_cast<UIBattery*>(uiElements[BATTERY].get());
		DirectX::XMFLOAT2 pos = battery->GetPosition();
		float posArr[2] = { pos.x, pos.y };
		if (ImGui::DragFloat2("BatteryUI Position", posArr, 1.0f)) {
			battery->SetPosition(posArr[0], posArr[1]);
		}
		DirectX::XMFLOAT2 scale = battery->GetScale();
		float scaleArr[2] = { scale.x, scale.y };
		if (ImGui::DragFloat2("BatteryUI Scale", scaleArr, 0.01f)) {
			battery->SetScale(scaleArr[0], scaleArr[1]);
		}
	}
	ImGui::Separator();
	{
		UIDustCount* dustCount = static_cast<UIDustCount*>(uiElements[DUSTCOUNT].get());
		DirectX::XMFLOAT2 pos = dustCount->GetPosition();
		float posArr[2] = { pos.x, pos.y };
		if (ImGui::DragFloat2("DustCountUI Position", posArr, 1.0f)) {
			dustCount->SetPosition(posArr[0], posArr[1]);
		}
		DirectX::XMFLOAT2 scale = dustCount->GetScale();
		float scaleArr[2] = { scale.x, scale.y };
		if (ImGui::DragFloat2("DustCountUI Scale", scaleArr, 0.01f)) {
			dustCount->SetScale(scaleArr[0], scaleArr[1]);
		}
		DirectX::XMFLOAT2 fontOffset = dustCount->GetFontOffset();
		float fontOffsetArr[2] = { fontOffset.x, fontOffset.y };
		if (ImGui::DragFloat2("DustCountUI Font Offset", fontOffsetArr, 1.0f)) {
			dustCount->SetFontOffset(fontOffsetArr[0], fontOffsetArr[1]);
		}
		DirectX::XMFLOAT2 addedScoreOffset = dustCount->GetAddedScoreOffset();
		float addedScoreOffsetArr[2] = { addedScoreOffset.x, addedScoreOffset.y };
		if (ImGui::DragFloat2("DustCountUI Added Score Offset", addedScoreOffsetArr, 1.0f)) {
			dustCount->SetAddedScoreOffset(addedScoreOffsetArr[0], addedScoreOffsetArr[1]);
		}
	}
	ImGui::Separator();
	{
		UiTimer* timer = static_cast<UiTimer*>(uiElements[TIMER].get());
		DirectX::XMFLOAT2 pos = timer->GetPosition();
		float posArr[2] = { pos.x, pos.y };
		if (ImGui::DragFloat2("TimerUI Position", posArr, 1.0f)) {
			timer->SetPosition(posArr[0], posArr[1]);
		}
		DirectX::XMFLOAT2 scale = timer->GetScale();
		float scaleArr[2] = { scale.x, scale.y };
		if (ImGui::DragFloat2("TimerUI Scale", scaleArr, 0.01f)) {
			timer->SetScale(scaleArr[0], scaleArr[1]);
		}
		DirectX::XMFLOAT2 fontOffset = timer->GetFontOffset();
		float fontOffsetArr[2] = { fontOffset.x, fontOffset.y };
		if (ImGui::DragFloat2("TimerUI Font Offset", fontOffsetArr, 1.0f)) {
			timer->SetFontOffset(fontOffsetArr[0], fontOffsetArr[1]);
		}
	}
	ImGui::Separator();
	{
		UiMyScore* myScore = static_cast<UiMyScore*>(uiElements[MYSCORE].get());
		DirectX::XMFLOAT2 pos = myScore->GetPosition();
		float posArr[2] = { pos.x, pos.y };
		if (ImGui::DragFloat2("MyScoreUI Position", posArr, 1.0f)) {
			myScore->SetPosition(posArr[0], posArr[1]);
		}
		DirectX::XMFLOAT2 scale = myScore->GetScale();
		float scaleArr[2] = { scale.x, scale.y };
		if (ImGui::DragFloat2("MyScoreUI Scale", scaleArr, 0.01f)) {
			myScore->SetScale(scaleArr[0], scaleArr[1]);
		}
		DirectX::XMFLOAT2 fontOffset = myScore->GetFontOffset();
		float fontOffsetArr[2] = { fontOffset.x, fontOffset.y };
		if (ImGui::DragFloat2("MyScoreUI Font Offset", fontOffsetArr, 1.0f)) {
			myScore->SetFontOffset(fontOffsetArr[0], fontOffsetArr[1]);
		}
	}
	ImGui::Separator();
	{
		UiHighScore* highScore = static_cast<UiHighScore*>(uiElements[HIGHSCORE].get());
		DirectX::XMFLOAT2 pos = highScore->GetPosition();
		float posArr[2] = { pos.x, pos.y };
		if (ImGui::DragFloat2("HighScoreUI Position", posArr, 1.0f)) {
			highScore->SetPosition(posArr[0], posArr[1]);
		}
		DirectX::XMFLOAT2 scale = highScore->GetScale();
		float scaleArr[2] = { scale.x, scale.y };
		if (ImGui::DragFloat2("HighScoreUI Scale", scaleArr, 0.01f)) {
			highScore->SetScale(scaleArr[0], scaleArr[1]);
		}
		DirectX::XMFLOAT2 fontOffset = highScore->GetFontOffset();
		float fontOffsetArr[2] = { fontOffset.x, fontOffset.y };
		if (ImGui::DragFloat2("HighScoreUI Font Offset", fontOffsetArr, 1.0f)) {
			highScore->SetFontOffset(fontOffsetArr[0], fontOffsetArr[1]);
		}
	}
	ImGui::Separator();
	{
		UiGarbageIcon* garbageIcon = static_cast<UiGarbageIcon*>(uiElements[GARBAGEICON].get());
		DirectX::XMFLOAT2 pos = garbageIcon->GetPosition();
		float posArr[2] = { pos.x, pos.y };
		if (ImGui::DragFloat2("GarbageIconUI Position", posArr, 1.0f)) {
			garbageIcon->SetPosition(posArr[0], posArr[1]);
		}
		DirectX::XMFLOAT2 scale = garbageIcon->GetScale();
		float scaleArr[2] = { scale.x, scale.y };
		if (ImGui::DragFloat2("GarbageIconUI Scale", scaleArr, 0.01f)) {
			garbageIcon->SetScale(scaleArr[0], scaleArr[1]);
		}
	}
	ImGui::Separator();
	{
		UIComboCounter* comboCounter = static_cast<UIComboCounter*>(uiElements[COMBOCOUNTER].get());
		DirectX::XMFLOAT2 pos = comboCounter->GetPosition();
		float posArr[2] = { pos.x, pos.y };
		if (ImGui::DragFloat2("ComboCounterUI Position", posArr, 1.0f)) {
			comboCounter->SetPosition(posArr[0], posArr[1]);
		}
		DirectX::XMFLOAT2 scale = comboCounter->GetScale();
		float scaleArr[2] = { scale.x, scale.y };
		if (ImGui::DragFloat2("ComboCounterUI Scale", scaleArr, 0.01f)) {
			comboCounter->SetScale(scaleArr[0], scaleArr[1]);
		}
		DirectX::XMFLOAT2 fontOffset = comboCounter->GetFontOffset();
		float fontOffsetArr[2] = { fontOffset.x, fontOffset.y };
		if (ImGui::DragFloat2("ComboCounterUI Font Offset", fontOffsetArr, 1.0f)) {
			comboCounter->SetFontOffset(fontOffsetArr[0], fontOffsetArr[1]);
		}
	}

	ImGui::End();
#endif
}
