#pragma once
#include "DxLib.h"
#include "CameraBase.h"

class Player;

// カメラクラス
class Camera : public CameraBase
{
public:
	Camera();	// コンストラクタ
	~Camera();	// デストラクタ

public:
	// 初期化～終了関数はCameraBaseからoverrideする
	// これでCameraBaseポインタ変数からCameraクラスの関数が呼べる
	void Init() override;
	void Load() override;
	void Start() override;
	void Step() override;
	void Update() override;
	void Draw() override;
	void Fin() override;

	void SetBaseBackOffset(float offset) { m_BaseBackOffset = offset; }
	void SetBaseUpOffset(float offset) { m_AssemblyBackOffset = offset; }
	void SetAssemblyBackOffset(float offset) { m_AssemblyBackOffset = offset; }
	void SetAssemblyUpOffset(float offset) { m_AssemblyUpOffset = offset; }
private:

	Player* m_TargetPlayer;
	bool m_IsAssembled;

	// ===== 通常時のオフセット =====
	float m_BaseBackOffset = 10.0f; // 通常時の後方距離
	float m_BaseUpOffset = 5.0f;  // 通常時の高さ

	// ===== 合体時の追加オフセット =====
	float m_AssemblyBackOffset = 5.0f; // 合体時にさらに引く距離
	float m_AssemblyUpOffset = 3.0f; // 合体時にさらに上げる高さ
};


