#include "UiDustCount.h"
#include "System/Graphics.h"
#include "Player.h"
#include "SceneGame.h"

UIDustCount::UIDustCount()
{
	// テクスチャの読み込み
	counter = std::make_unique<Sprite>("Data/Sprite/UI/DustUI.png");

	auto& graphics = Graphics::Instance();
	auto device = graphics.GetDevice();
	auto context = graphics.GetDeviceContext();

	spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Data/Font/Font1.spritefont");

	// 初期位置とサイズ
	SetPosition(1030.0f, -12.0f);
	SetScale(0.5f, 0.5f);
}

UIDustCount::~UIDustCount()
{
}

void UIDustCount::Update(float elapsedTime)
{
    // 準備
    int count = SceneGame::Instance().GetGomiCount();

	// カウントが変わったときだけテキストを更新
    if (count != lastCount)
    {
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
}

void UIDustCount::Render(const RenderContext& rc)
{
	// 親の描画
    float dx = position.x;
    float dy = position.y;

    counter->Render(rc, dx, dy, 0.1f,
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
