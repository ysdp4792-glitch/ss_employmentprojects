#include "DxLib.h"
#include "Player/PlayerManager.h"
#include "Camera/CameraManager.h"
#include "Scene/SceneManager.h"
#include "Input/Input.h"
#include "Common/Common.h"
#include "EffekseerForDXLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    ChangeWindowMode(TRUE);

    SetZBufferBitDepth(24);

    SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);

    if (DxLib_Init() == -1)
    {
        return -1;
    }

    // ===== Effekseer初期化 =====
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);

    Effekseer_Init(8000);

    SetDrawScreen(DX_SCREEN_BACK);

    SceneManager::CreateInstance();
    SceneManager* sceneManager = SceneManager::GetInstance();
    sceneManager->Init();

    Input::Init();

    while (ProcessMessage() >= 0)
    {
        Sleep(1);

        ClearDrawScreen();

        Input::Update();

        // Effekseer更新
        UpdateEffekseer3D();

        sceneManager->Update();

        Input::Draw();

        // Effekseer描画
        DrawEffekseer3D();

        if (CheckHitKey(KEY_INPUT_ESCAPE)) break;

        ScreenFlip();
    }

    SceneManager::DeleteInstance();

    Input::Fin();

    DxLib_End();

    return 0;
}