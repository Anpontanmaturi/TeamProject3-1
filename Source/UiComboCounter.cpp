#include "UiComboCounter.h"
#include "System/Graphics.h"
#include "Player.h"
#include "SceneGame.h"

UIComboCounter::UIComboCounter()
{
    // テクスチャの読み込み
    combo = std::make_unique<Sprite>("Data/Sprite/UI/comboCount.png");

    auto& graphics = Graphics::Instance();
    auto device = graphics.GetDevice();
    auto context = graphics.GetDeviceContext();

    spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
    spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Data/Font/Font1.spritefont");

    // 初期位置とサイズ
    SetPosition(1170.0f, 145.0f);
    SetScale(0.6f, 0.6f);
}

UIComboCounter::~UIComboCounter()
{
    spriteFont.reset();
    spriteBatch.reset();
}

void UIComboCounter::Update(float elapsedTime)
{
    // 準備
    int count = SceneGame::Instance().GetCombo();

    lastCount = count;
    text = std::to_wstring(count);

    fontScale = { scale.x * scaleMag, scale.y * scaleMag };

    // フォントの描画位置を計算

    DirectX::XMVECTOR sizeVec = spriteFont->MeasureString(text.c_str());
    float baseWidth = DirectX::XMVectorGetX(sizeVec);

    // 実際の描画幅を計算
    float actualWidth = baseWidth * fontScale.x;

    fontPos.x = position.x + (FontOffset.x * scale.x) - actualWidth;
    fontPos.y = position.y + (FontOffset.y * scale.y);

}

void UIComboCounter::Render(const RenderContext& rc)
{
    // 親の描画
    float dx = position.x;
    float dy = position.y;

    combo->Render(rc, dx, dy, 0.1f,
        Origin_W * scale.x, Origin_H * scale.y, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f);

    // 文字の描画
    spriteBatch->Begin();

    spriteFont->DrawString(
        spriteBatch.get(),
        text.c_str(),
        fontPos,
        fontColor,
        0.0f,                   // 回転
        DirectX::XMFLOAT2(0, 0), // 原点
        fontScale       // スケールを画像に合わせる
    );

    spriteBatch->End();
}
