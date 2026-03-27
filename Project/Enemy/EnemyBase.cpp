#include <DxLib.h>
#include "EnemyBase.h"
#include "../Collision/CollisionManager.h"
#include "../Stage/Stage.h"
#include "../MyMath/MyMath.h"
#include <algorithm>
#include <cmath>

// std::clamp が使えない場合の代替 clamp 関数を定義
template <typename T>
T clamp(T v, T lo, T hi)
{
    return (v < lo) ? lo : (v > hi) ? hi : v;
}

EnemyBase::EnemyBase()
{
	m_Handle = 0;
	m_Pos = VGet(0.0f, 0.0f, 0.0f);
	m_Rot = VGet(0.0f, 0.0f, 0.0f);
	m_Scale = VGet(0.0f, 0.0f, 0.0f);
	m_Move = VGet(0.0f, 0.0f, 0.0f);
}

EnemyBase::~EnemyBase()
{
	Fin();
}

void EnemyBase::Init()
{
}

void EnemyBase::Load()
{
}

void EnemyBase::Start()
{

}

void EnemyBase::Step()
{
}

void EnemyBase::Update()
{
	m_Pos = MyMath::VecAdd(m_Pos, m_Move);

	MV1SetPosition(m_Handle, m_Pos);
	MV1SetRotationXYZ(m_Handle, m_Rot);
	MV1SetScale(m_Handle, m_Scale);
}

void EnemyBase::Draw()
{
	// モデル描画（既存）
	if (m_Handle >= 0) {
		MV1SetPosition(m_Handle, m_Pos);
		MV1SetRotationXYZ(m_Handle, m_Rot);
		MV1SetScale(m_Handle, m_Scale);
		MV1DrawModel(m_Handle);
	}
}

void EnemyBase::Fin()
{
	if (m_AABB)
	{
		if (CollisionManager::GetInstance())
		{
			CollisionManager::GetInstance()->DeleteAABB(m_AABB);
		}
		m_AABB = nullptr;
	}
	

	if (m_Handle >= 0)
	{
		MV1DeleteModel(m_Handle);
		m_Handle = -1;
	}
}

void EnemyBase::Damage(int damage)
{
	m_HP -= damage;
	if (m_HP <= 0)
	{
		m_IsDead = true;
	}
}

void EnemyBase::HitStage(Stage* stage)
{
	if (!stage) return;

	// 簡易的なライン判定で地形と当たっているかを判定し、Y を補正する処理。
	// Player::HitStage と同様の処理を行う（敵はカプセルを持っていない場合があるため、
	// 敵の頭上から床へ向けてラインを引く簡易手法を使う）。
	MV1_COLL_RESULT_POLY_DIM hitResult = stage->GetHitResult();
	for (int i = 0; i < hitResult.HitNum; i++) {
		MV1_COLL_RESULT_POLY polygon = hitResult.Dim[i];
		if (polygon.Normal.y <= 0.1f) continue;

		// lineStart を敵の少し上、lineEnd を現在の位置にして判定
		VECTOR lineStart = VAdd(m_Pos, VGet(0.0f, 5.0f, 0.0f));
		VECTOR lineEnd = m_Pos;

		HITRESULT_LINE lineRes = HitCheck_Line_Triangle(lineStart, lineEnd,
			polygon.Position[0], polygon.Position[1], polygon.Position[2]);

		if (!lineRes.HitFlag) continue;

		// 高さだけ修正（地形の高さに合わせる）
		m_Pos.y = lineRes.Position.y;
		// 少し持ち上げる等のリアクション（敵の挙動に応じて調整）
		m_Move.y = 0.0f;
		// 一度ヒットしたら十分
		break;
	}
}

// HUD描画部分の修正
// GetFontDefault() の代わりに DxLib のデフォルトフォントハンドル取得方法を使用
// DxLib では CreateFontToHandle(NULL, ...) でデフォルトフォントを作成可能

void EnemyBase::DrawHUD()
{
	if (m_MaxHP <= 0) return;

	float rate = (float)m_HP / (float)m_MaxHP;
	rate = clamp(rate, 0.0f, 1.0f);

	// ワールド上のHPバー基準位置（敵の頭上）
	VECTOR worldPos = VAdd(m_Pos, VGet(0.0f, 7.0f, 0.0f));

	// バーのワールドサイズ（必要に応じて調整）
	const float barWidth = 2.4f;   // ワールド単位の幅
	const float barHeight = 0.16f; // 見やすさを調整する厚み（垂直方向）
	const float barOffsetY = 0.0f; // バーの垂直オフセット追加が必要なら調整

	// 回転に合わせてバーを傾ける処理（Y軸回転のみ）
	float yaw = m_Rot.y;

	// DxLib の回転が度数かラジアンか不明な場合を考慮する簡単な判定:
	// 角度の絶対値が 2π より大きければ度（deg）とみなしラジアンに変換する
	const float TWO_PI = 6.28318530718f;
	if (fabsf(yaw) > TWO_PI) {
		yaw = yaw * (3.14159265358979323846f / 180.0f);
	}

	float cosA = cosf(yaw);
	float sinA = sinf(yaw);

	// ローカル +X 軸（右方向）を Y 軸回転で回したベクトルを作る
	VECTOR rightDir = VGet(cosA, 0.0f, sinA);
	VECTOR halfOffset = VScale(rightDir, barWidth * 0.5f);

	// 左右の端点（回転を反映）
	VECTOR left = VSub(worldPos, halfOffset);
	VECTOR right = VAdd(worldPos, halfOffset);

	// 垂直オフセット（バーの厚み表現）
	VECTOR up = VGet(0.0f, barHeight, 0.0f);

	// 背景（グレー）と前景（色はHPに応じて変化）を 3D ラインで描画する
	int bgColor = GetColor(50, 50, 50);

	// 前景色を緑->赤で補間（rate==1 => 緑、rate==0 => 赤）
	int r = (int)((1.0f - rate) * 255.0f + 0.5f);
	int g = (int)(rate * 255.0f + 0.5f);
	int fgColor = GetColor(r, g, 0);

	// 背景（上下2本で少し判別しやすくする）
	DrawLine3D(VAdd(left, VGet(0, barOffsetY, 0)), VAdd(right, VGet(0, barOffsetY, 0)), bgColor);
	DrawLine3D(VAdd(left, VAdd(up, VGet(0, barOffsetY, 0))), VAdd(right, VAdd(up, VGet(0, barOffsetY, 0))), bgColor);

	// 前景（割合に応じた長さ）: left -> filled
	VECTOR filled = VAdd(left, VScale(VSub(right, left), rate));
	DrawLine3D(VAdd(left, VGet(0, barOffsetY, 0)), VAdd(filled, VGet(0, barOffsetY, 0)), fgColor);
	DrawLine3D(VAdd(left, VAdd(up, VGet(0, barOffsetY, 0))), VAdd(filled, VAdd(up, VGet(0, barOffsetY, 0))), fgColor);

	// バー両端の縁取り（小さな縦線）
	DrawLine3D(left, VAdd(left, up), bgColor);
	DrawLine3D(right, VAdd(right, up), bgColor);

	//（オプション）HP数値をワールド上に小さく表示する場合は DrawFormatString を使うために
	// ConvWorldPosToScreenPos が必要。今回は案2なので省略しています。
}
