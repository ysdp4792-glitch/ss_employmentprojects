#include "DxLib.h"
#include "CollisionCapsule.h"
#include "../MyMath/MyMath.h"
#include "math.h"

#define CAPSULE_H 2

CollisionCapsule::CollisionCapsule()  
{
	m_TargetPos = nullptr;
	m_LocalPos = VGet(0.0f, 0.0f, 0.0f);
	m_RotationY = 0.0f;
	m_Radius = 0.0f;
	m_CapsulePos1 = VGet(0.0f, 0.0f, 0.0f);
	m_CapsulePos2 = VGet(0.0f, 0.0f, 0.0f);
	m_HitPolyDim = {};
}

CollisionCapsule::~CollisionCapsule()
{
}

void CollisionCapsule::Draw()
{
	// ワールド上の始点・終点を取得して描画（GetStartPoint/GetEndPoint は動的に計算するように実装済み）
	VECTOR start = GetStartPoint();
	VECTOR end = GetEndPoint();

	// キャッシュも更新しておく（他所で m_CapsulePos1/2 を参照している可能性があるため）
	m_CapsulePos1 = start;
	m_CapsulePos2 = end;

	DrawCapsule3D(m_CapsulePos1, m_CapsulePos2, m_Radius, 10, GetColor(255,255,255), GetColor(255, 255, 255), false); 
}

VECTOR CollisionCapsule::GetWorldPos()
{
    return MyMath::VecAdd(*m_TargetPos, m_LocalPos); 
}

// カプセルの開始点を取得（動的に計算して返す）
VECTOR CollisionCapsule::GetStartPoint() const
{
	if (!m_TargetPos)
	{
		return VGet(0.0f, 0.0f, 0.0f);
	}

	// カプセル中心のワールド位置
	VECTOR center = MyMath::VecAdd(*m_TargetPos, m_LocalPos);

	// 高さの半分（CAPSULE_H はカプセルの全長）
	float halfH = CAPSULE_H * 0.5f;

	// デフォルトは縦方向（Y軸）に伸びるカプセルとする
	VECTOR start = VAdd(center, VGet(0.0f, -halfH, 0.0f));

	return start;
}

// カプセルの終了点を取得（動的に計算して返す）
VECTOR CollisionCapsule::GetEndPoint() const
{
	if (!m_TargetPos)
	{
		return VGet(0.0f, 0.0f, 0.0f);
	}

	VECTOR center = MyMath::VecAdd(*m_TargetPos, m_LocalPos);
	float halfH = CAPSULE_H * 0.5f;

	VECTOR end = VAdd(center, VGet(0.0f, halfH, 0.0f));

	return end;
}

bool CollisionCapsule::IsHit(CollisionCapsule* a, CollisionCapsule* b)
{
	if (!a || !b) return false;

	VECTOR aStart = a->GetStartPoint();
	VECTOR aEnd = a->GetEndPoint();
	float aRadius = a->GetRadius();

	VECTOR bStart = b->GetStartPoint();
	VECTOR bEnd = b->GetEndPoint();
	float bRadius = b->GetRadius();

	// 2本の線分の最短距離を求める
	float distance = MyMath::SegmentToSegmentDistance(aStart, aEnd, bStart, bEnd); 

	// 半径の合計より近ければ当たっていると判定
	return distance <= (aRadius + bRadius);
}

bool CollisionCapsule::CheckCollisionSphere(VECTOR spherePos, float sphereRadius)
{
	// capsule の線分端点を動的に取得
	VECTOR capsuleStart = GetStartPoint();
	VECTOR capsuleEnd = GetEndPoint();
	float capsuleRadius = GetRadius();

	// カプセルの中心線と球の中心の最短距離を計算
	// MyMath::SegmentToSegmentDistance を利用（第二セグメントを点として渡す）
	float distance = MyMath::SegmentToSegmentDistance(capsuleStart, capsuleEnd, spherePos, spherePos);

	// 半径の合計より近ければ衝突していると判定
	return distance <= (capsuleRadius + sphereRadius);
}

// カプセルの始点ワールド座標を取得（回転や local オフセットを考慮して動的に返す）
VECTOR CollisionCapsule::GetWorldPosStart() const
{
	return GetStartPoint();
}

VECTOR CollisionCapsule::GetWorldPosEnd() const
{
	return GetEndPoint();
}
