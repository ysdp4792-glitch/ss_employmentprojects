#include "DxLib.h"
#include "Input.h"
#include <cmath> 


int Input::m_InputState = 0;
int Input::m_PrevInputState = 0;
AnalogState Input::m_Analog = {};

void Input::Init()
{
    m_InputState = 0;
    m_PrevInputState = 0;
    m_Analog = {};
}

void Input::Update()
{
    m_PrevInputState = m_InputState;
    m_InputState = 0;

    // =========================
    // キーボード（保険）
    // =========================
    if (CheckHitKey(KEY_INPUT_W)) m_InputState |= KEY_UP;
    if (CheckHitKey(KEY_INPUT_S)) m_InputState |= KEY_DOWN;
    if (CheckHitKey(KEY_INPUT_A)) m_InputState |= KEY_LEFT;
    if (CheckHitKey(KEY_INPUT_D)) m_InputState |= KEY_RIGHT;

    if (CheckHitKey(KEY_INPUT_Z))     m_InputState |= KEY_Z;
    if (CheckHitKey(KEY_INPUT_C))     m_InputState |= KEY_C;
    if (CheckHitKey(KEY_INPUT_1))     m_InputState |= KEY_1;
    if (CheckHitKey(KEY_INPUT_SPACE)) m_InputState |= KEY_SPACE;
    if (CheckHitKey(KEY_INPUT_LSHIFT))m_InputState |= KEY_SHIFT;
    if (CheckHitKey(KEY_INPUT_L))     m_InputState |= KEY_L;

    // =========================
    // Xbox コントローラー
    // =========================
    int pad = GetJoypadInputState(DX_INPUT_PAD1);

    // ボタン
    if (pad & PAD_INPUT_R)   m_InputState |= KEY_Z;       // RB：メイン攻撃
    if (pad & PAD_INPUT_X)   m_InputState |= KEY_C;       // X ：サブ攻撃
    if (pad & PAD_INPUT_Y)   m_InputState |= KEY_1;       // Y ：武装切替
    if (pad & PAD_INPUT_A)   m_InputState |= KEY_SPACE;   // A ：ブースト
    if (pad & PAD_INPUT_L)   m_InputState |= KEY_SHIFT;   // LB：ガード
    //if (pad & PAD_INPUT_)m_InputState |= KEY_L;       // View：ロックオン

    // 十字キー（微調整移動）
    if (pad & PAD_INPUT_UP)    m_InputState |= KEY_UP;
    if (pad & PAD_INPUT_DOWN)  m_InputState |= KEY_DOWN;
    if (pad & PAD_INPUT_LEFT)  m_InputState |= KEY_LEFT;
    if (pad & PAD_INPUT_RIGHT) m_InputState |= KEY_RIGHT;

    // =========================
 // アナログ（左スティック）移動
 // =========================
    int lx, ly;
    GetJoypadAnalogInput(&lx, &ly, DX_INPUT_PAD1);

    m_Analog.moveX = DeadZone(lx / 1000.0f);
    m_Analog.moveY = DeadZone(-ly / 1000.0f);


    // =========================
    // 右スティック（カメラ）
    // =========================
    int rx, ry;
    GetJoypadAnalogInputRight(&rx, &ry, DX_INPUT_PAD1);

    m_Analog.yaw = DeadZone(-rx / 1000.0f);  // カメラ左右
    m_Analog.pitch = DeadZone(-ry / 1000.0f);  // カメラ上下
}

void Input::Draw()
{
}

void Input::Fin()
{
}

bool Input::IsInputKey(InputKey key)
{
    return (m_InputState & key) != 0;
}

bool Input::IsTriggerKey(InputKey key)
{
    return (m_InputState & key) && !(m_PrevInputState & key);
}

const AnalogState& Input::GetAnalog()
{
    return m_Analog;
}

float Input::DeadZone(float v)
{
    return (std::fabs(v) < 0.15f) ? 0.0f : v;
}

// POV（方向キー）の値を取得するラッパー関数を定義
int Input::GetJoypadPOV(int padInputNo)
{
    // DxLibのジョイパッド状態取得構造体
    DINPUT_JOYSTATE state;
    // ジョイパッドの状態を取得
    if (GetJoypadDirectInputState(padInputNo, &state) == 0)
    {
        // POV[0]に方向キーの値が入る（0xffffffffは入力なし）
        return (state.POV[0] == 0xffffffff) ? -1 : (int)state.POV[0];
    }
    return -1;
}