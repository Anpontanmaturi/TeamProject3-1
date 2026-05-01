#pragma once
#include <vector>
#include "UiBase.h"
#include <memory>

// UIマネージャー
class UIManager
{
private:
	UIManager(){}
	~UIManager(){}

public:
	// インスタンス取得
	static UIManager& Instance() {
		static UIManager instance;
		return instance;
	}

	// 初期化
	void Init();

	// 更新
	void Update(float elapsedTime);

	// 描画
	void Render(const RenderContext& rc);

	// デバッグ用
	void DrawDebugGUI();

private:
	// UI
	std::vector<std::unique_ptr<UIBase>> uiElements;
};
