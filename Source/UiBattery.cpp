#include "UiBattery.h"
#include "Player.h"

UIBattery::UIBattery()
{
	// テクスチャの読み込み
	frame = std::make_unique<Sprite>("Data/Sprite/UI/Battery/frame.png");
	fill = std::make_unique<Sprite>("Data/Sprite/UI/Battery/battery.png");
	back = std::make_unique<Sprite>("Data/Sprite/UI/Battery/back.png");

	// 初期位置とサイズ
	SetPosition(50.0f, 50.0f);
	//SetScale(0.5f, 0.5f);
}

UIBattery::~UIBattery()
{
}

void UIBattery::Update(float elapsedTime)
{
	// プレイヤーのエネルギー量を取得
	float nowEnergy = Player::Instance().GetEnergy();
	float maxEnergy = Player::Instance().GetMaxEnergy();

	if(maxEnergy > 0.0f){
		energy = nowEnergy / maxEnergy;
	}

	energy = (energy < 0.0f) ? 0.0f : (energy > 1.0f ? 1.0f : energy);
}

void UIBattery::Render(const RenderContext& rc)
{
    // 描画パラメータ
    float dx = position.x;
    float dy = position.y;
    float dw = baseWidth * scale.x;
    float dh = baseHeight * scale.y;

    // 背景
    back->Render(rc, dx, dy, 0.1f, dw, dh, 0.0f, color.x, color.y, color.z, color.w);

    // 中身
    float sw = baseWidth * energy;
    float sh = baseHeight;
    float currentDw = dw * energy;

    fill   ->Render(rc,
        dx, dy,       // 表示位置
        0.05f,        // 奥行
        currentDw, dh,// 表示サイズ
        0.0f, 0.0f,   // 画像切り抜き開始位置 
        sw, sh,       // 画像切り抜きサイズ
        0.0f,         // 回転
        color.x, color.y, color.z, color.w
    );

    // フレーム
    frame->Render(rc, dx, dy, 0.0f, dw, dh, 0.0f, color.x, color.y, color.z, color.w);
}