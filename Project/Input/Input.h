#pragma once

// 入力ボタン定義
enum InputKey
{
	KEY_UP		= (1 << 0),	// 0x0000 0000 0001
	KEY_DOWN	= (1 << 1),	// 0x0000 0000 0010
	KEY_LEFT	= (1 << 2),	// 0x0000 0000 0100
	KEY_RIGHT	= (1 << 3),	// 0x0000 0000 1000
	KEY_Z		= (1 << 4),	// 0x0000 0001 0000
	KEY_A		= (1 << 5),	// 0x0000 0010 0000
	KEY_W		= (1 << 6),	// 0x0000 0100 0000
	KEY_S		= (1 << 7),	// 0x0000 1000 0000
	KEY_D		= (1 << 8),	// 0x0001 0000 0000
	KEY_1		= (1 << 9),	// 0x0010 0000 0000
	KEY_L       = (1 << 10),// 0x0100 0000 0000
	KEY_SPACE   = (1 << 11),// 0x1000 0000 0000
	KEY_G       = (1 << 12),
	KEY_N       = (1 << 13),
	KEY_SHIFT   = (1 << 14),
	KEY_C       = (1 << 15),
};

// アナログ入力（ロボット操縦用）
struct AnalogState
{
	float moveX;   // 左右移動
	float moveY;   // 前後移動
	float yaw;     // 旋回
	float pitch;   // 視点上下
};


class Input
{
public:
	static void Init();
	static void Update();
	static void Draw();
	static void Fin();

	static bool IsInputKey(InputKey key);
	static bool IsTriggerKey(InputKey key);

	static const AnalogState& GetAnalog();
	static int GetJoypadPOV(int padInputNo);
private:
	static float DeadZone(float v);

private:
	static int m_InputState;
	static int m_PrevInputState;
	static AnalogState m_Analog;
};




