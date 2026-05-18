#pragma once
#include "UiBase.h"
#include "System/Sprite.h"
#include <memory>

class UiGarbageIcon : public UIBase
{
public:
    UiGarbageIcon();
    virtual ~UiGarbageIcon() override;

    virtual void Update(float elapsedTime) override;
    virtual void Render(const RenderContext& rc) override;

private:
    // 2種類の画像ポインタを用意する
    std::unique_ptr<Sprite> sprEmpty; // ガラクタを持っていない時の画像
    std::unique_ptr<Sprite> sprHave;  // ガラクタを持っている時の画像

    bool hasGarbage = false; // 現在持っているかどうかのフラグ

    // 画像の元のサイズ
    const float Origin_W = 190.0f;
    const float Origin_H = 190.0f;
};