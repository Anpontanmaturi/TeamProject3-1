// SceneResult.cpp



#include "SceneResult.h"
#include "System/Graphics.h"
#include "System/Input.h"
#include "SceneTitle.h"
#include <imgui.h>
#include <SceneManager.h>
#include <SceneGame.h>
#include "ScoreManager.h"

//#include <Instance.h>

void SceneResult::Initialize() {
    //スプライト初期化
    sprite = new Sprite("Data/Sprite/gameclear.png");
    resultAu = Audio::Instance().LoadAudioSource("Data/Sound/tanoshiibouken.wav");
    resultAu->Play(true);
	// ランキングUIの生成
    rankingUI = std::make_unique<UiRanking>(); 
}
void SceneResult::Finalize() {

    if (rankingUI != nullptr)
    {
        rankingUI->Finalize();
    }

    rankingUI.reset();

    //スプライト終了化
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;

    }

    // 音楽停止
    resultAu->Stop();
}



void SceneResult::Update(float elapsedTime)
{

    // ランキングUIの文字更新や座標計算を呼ぶ
    if (rankingUI)
    {
        rankingUI->Update(elapsedTime);
    }

    // 入力でシーンを戻すなど
    //Input& input = Input::Instance();
    GamePad& gamePad = Input::Instance().GetGamePad();
    const GamePadButton anyButton = GamePad::BTN_A;
    if ((GetAsyncKeyState(VK_SPACE) & 0x8000))
    {
        // 例：タイトルに戻る
        SceneManager::Instance().ChangeScene(new SceneTitle());
    }
}




void SceneResult::Render() {

    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    RenderState* renderState = graphics.GetRenderState();

    //描画準備
    RenderContext rc;
    rc.deviceContext = dc;
    rc.renderState = graphics.GetRenderState();

    //2Dスプライト描画
    {
        //タイトル描画
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        sprite->Render(rc, 0, 0, 0, screenWidth, screenHeight, 0, 1, 1, 1, 1);

		// ランキングUIの描画
        if (rankingUI)
        {
            rankingUI->Render(rc);
        }

    }
}


void SceneResult::DrawGUI()
{
    // 今は何も描かなくてOK
}