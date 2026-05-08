#include "UiDustCount.h"
#include "System/Graphics.h"
#include "Player.h"

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

    // フォントの描画位置を計算
    DirectX::XMFLOAT2 fontPos;
    fontPos.x = dx + (FontOffset.x * scale.x);
    fontPos.y = dy + (FontOffset.y * scale.y);

	fontScale = { scale.x * 3.0f, scale.y * 3.0f };

    // 文字の描画
    spriteBatch->Begin();

    // とりあえずテスト用の数字を表示
    std::wstring text = L"1"; 
    
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
