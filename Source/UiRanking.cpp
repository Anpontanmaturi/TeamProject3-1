#include "UiRanking.h"
#include "ScoreManager.h"	
#include "System/Graphics.h"

UiRanking::UiRanking()
{
	// SpriteBatchとSpriteFontの初期化(フォント用)
	auto& graphics = Graphics::Instance();
	spriteBatch = std::make_unique<DirectX::SpriteBatch>(graphics.GetDeviceContext());
	spriteFont = std::make_unique<DirectX::SpriteFont>(graphics.GetDevice(), L"Data/Font/Font1.spritefont");
	// 初期位置とサイズ
	SetPosition(0.0f, 100.0f);
	SetScale(0.7f, 0.7f);
}

UiRanking::~UiRanking()
{
    spriteFont.reset();
    spriteBatch.reset();
}

void UiRanking::Update(float elapsedTime)
{
	if (!spriteFont) return;

	// ランキング配列を取得
	const auto& scores = ScoreManager::Instance().GetTopScores();

	// 表示用リストをクリアして作り直す
	rankingTexts.clear();
	for (size_t i = 0; i < scores.size(); ++i)
	{
		std::wstring rankName;
		if (i == 0)      rankName = L"1st";
		else if (i == 1) rankName = L"2nd";
		else if (i == 2) rankName = L"3rd";
		else             rankName = std::to_wstring(i + 1) + L"th";

		rankingTexts.push_back(rankName + L" : " + std::to_wstring(scores[i]));
	}

	// スケール設定
	fontScale = { scale.x * scaleMag, scale.y * scaleMag };
}

void UiRanking::Render(const RenderContext& rc)
{
    if (rankingTexts.empty()) return;

    spriteBatch->Begin();

    // 行間
    float lineSpacing = lineSpace * scale.y;

    for (size_t i = 0; i < rankingTexts.size(); ++i)
    {
        // 1行ごとに Y 座標を（i * 行間）分だけ下にずらしていく
        DirectX::XMFLOAT2 currentPos = {
            position.x + (FontOffset.x * scale.x),
            position.y + (FontOffset.y * scale.y) + (i * lineSpacing)
        };

        // 順位（i）に応じて文字の色を変えて豪華にする
        DirectX::XMVECTOR color = DirectX::Colors::White;
        if (i == 0)      color = DirectX::Colors::Gold;       // 1位：金
        else if (i == 1) color = DirectX::Colors::Silver;     // 2位：銀
        else if (i == 2) color = DirectX::Colors::Chocolate;  // 3位：ブロンズ（銅）

        spriteFont->DrawString(
            spriteBatch.get(),
            rankingTexts[i].c_str(),
            currentPos,
            color,
            0.0f,
            DirectX::XMFLOAT2(0, 0),
            fontScale
        );
    }

    spriteBatch->End();
}