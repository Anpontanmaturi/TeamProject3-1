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
    // 1. 親となるアイコン画像の描画
    float dx = DustUI_position.x;
    float dy = DustUI_position.y;

    counter->Render(rc, dx, dy, 0.1f,
        Origin_W * DustUI_scale.x, Origin_H * DustUI_scale.y, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f);

    // 2. フォントの描画位置を計算（親の座標 + オフセット）
    // 親のスケールも考慮するとより正確になります
    DirectX::XMFLOAT2 fontPos;
    fontPos.x = dx + (FontOffset.x * DustUI_scale.x);
    fontPos.y = dy + (FontOffset.y * DustUI_scale.y);

    // 3. 文字の描画
    spriteBatch->Begin();

    // とりあえずテスト用の数字を表示
    std::wstring text = L"x 10";

    spriteFont->DrawString(
        spriteBatch.get(),
        text.c_str(),
        fontPos,
        DirectX::Colors::White,
        0.0f,                   // 回転
        DirectX::XMFLOAT2(0, 0), // 原点
        DustUI_scale.x          // スケールを画像に合わせる
    );

    spriteBatch->End();
}
