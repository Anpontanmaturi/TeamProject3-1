#include "UiDustCount.h"
#include "System/Graphics.h"
#include "Player.h"
#include <SceneGame.h>

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
	SetPosition(880.0f, -19.0f);
	SetScale(0.8f, 0.8f);
}

UIDustCount::~UIDustCount()
{
}

void UIDustCount::Update(float elapsedTime)
{
	
}

void UIDustCount::Render(const RenderContext& rc)
{
	// 親の描画
    float dx = position.x;
    float dy = position.y;

    counter->Render(rc, dx, dy, 0.1f,
        Origin_W * scale.x, Origin_H * scale.y, 0.0f, 
        1.0f, 1.0f, 1.0f, 1.0f);

	fontScale = { scale.x * 2.5f, scale.y * 2.5f };

    // データの準備
    int count = SceneGame::Instance().GetGomiCount();
    std::wstring text = std::to_wstring(count);

    // フォントの描画位置を計算

    DirectX::XMVECTOR sizeVec = spriteFont->MeasureString(text.c_str());
    float baseWidth = DirectX::XMVectorGetX(sizeVec);

    // 実際の描画幅を計算
    float actualWidth = baseWidth * fontScale.x;

    DirectX::XMFLOAT2 fontPos;
    fontPos.x = dx + (FontOffset.x * scale.x) - actualWidth;
    fontPos.y = dy + (FontOffset.y * scale.y);

    // 文字の描画
    spriteBatch->Begin();

    
    spriteFont->DrawString(
        spriteBatch.get(),
        text.c_str(),
        fontPos,
        DirectX::Colors::White,
        0.0f,                   // 回転
        DirectX::XMFLOAT2(0, 0), // 原点
        fontScale       // スケールを画像に合わせる
    );

    spriteBatch->End();
}
