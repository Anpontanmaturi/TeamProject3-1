#include "UiBattery.h"
#include "Player.h"

UIBattery::UIBattery()
{
    // テクスチャの読み込み
    frame = std::make_unique<Sprite>("Data/Sprite/UI/frame.png");
    fill = std::make_unique<Sprite>("Data/Sprite/UI/battery.png");
    back = std::make_unique<Sprite>("Data/Sprite/UI/back.png");

    // 初期位置とサイズ
    SetPosition(780.0f, 0.0f);
    SetScale(1.0f, 0.5f);
}

UIBattery::~UIBattery()
{
}

void UIBattery::Update(float elapsedTime)
{
    // プレイヤーのエネルギー量を取得
    float nowEnergy = Player::Instance().GetEnergy();
    float maxEnergy = Player::Instance().GetMaxEnergy();

    if (maxEnergy > 0.0f) {
        energy = nowEnergy / maxEnergy;
    }

    energy = (energy < 0.0f) ? 0.0f : (energy > 1.0f ? 1.0f : energy);
}

void UIBattery::Render(const RenderContext& rc)
{
    // 描画パラメータ
    float dx = position.x;
    float dy = position.y;

    float backOffX = (FRAME_W - BACK_W) * 0.5f;
    float backOffY = (FRAME_H - BACK_H) * 0.5f;

    // 背景
    back->Render(rc, dx + backOffX, dy + backOffY, 0.1f,
        BACK_W * scale.x, BACK_H * scale.y, 0.0f, color.x, color.y, color.z, color.w);

    // 中身
    float fillOffX = (FRAME_W - FILL_W) * 0.5f;
    float fillOffY = (FRAME_H - FILL_H) * 0.5f;

    // エネルギー量に応じて、描画する幅とテクスチャの切り抜き位置を計算
    float missingW = FILL_W * (1.0f - energy);
    float drawX = dx + fillOffX + (missingW * scale.x) + (adjX * scale.x);
    float drawW = (FILL_W * scale.x) * energy;
    float srcX = missingW;
    float srcW = FILL_W * energy;

    // 中身の描画
    fill->Render(rc,
        drawX, dy + fillOffY,         // ずらした座標
        0.05f,                        // 奥行
        drawW, FILL_H * scale.y,      // 残りの幅で表示
        srcX, 0.0f,                   // ずらしたテクスチャ座標
        srcW, FILL_H,                 // 残りの幅だけ切り出す
        0.0f,                         // 角度
        color.x, color.y, color.z, color.w
    );

    // フレーム
    frame->Render(rc, dx, dy, 0.0f, FRAME_W * scale.x, FRAME_H * scale.y, 0.0f, color.x, color.y, color.z, color.w);
}