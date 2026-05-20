#include "UiGabageIcon.h"
#include "Player.h"

UiGarbageIcon::UiGarbageIcon()
{
	sprEmpty = std::make_unique<Sprite>("Data/Sprite/UI/Item/emptyItem.png");
	sprHave = std::make_unique<Sprite>("Data/Sprite/UI/Item/haveItem.png");
	// 初期位置とサイズ
	SetPosition(1065.0f, 145.0f);
	SetScale(0.6f, 0.6f);
}

UiGarbageIcon::~UiGarbageIcon()
{
    sprEmpty.reset();
    sprHave.reset();
}

void UiGarbageIcon::Update(float elapsedTime)
{
    // プレイヤーのガラクタ所持数をチェックしてフラグを切り替える
    if (Player::Instance().GetGarbageCount() > 0)
    {
        hasGarbage = true;
    }
    else
    {
        hasGarbage = false;
    }
}

void UiGarbageIcon::Render(const RenderContext& rc)
{
    // フラグに応じて描画するスプライトをスイッチする
    if (hasGarbage)
    {
        if (sprHave)
        {
            sprHave->Render(rc, position.x, position.y, 0.0f,
                Origin_W*scale.x, Origin_H*scale.y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
        }
    }
    else
    {
        if (sprEmpty)
        {
            sprEmpty->Render(rc, position.x, position.y, 0.0f,
                Origin_W * scale.x, Origin_H * scale.y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
        }
    }
}