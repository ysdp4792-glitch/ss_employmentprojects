#include "DxLib.h"
#include "CollisionSphere.h"
#include "CollisionCapsule.h"
#include "../MyMath/MyMath.h"

// コンストラクタ
CollisionSphere::CollisionSphere()
{
	m_TargetPos = nullptr;
	m_LocalPos = VGet(0.0f, 0.0f, 0.0f);
	m_Radius = 0.0f;
}

// デストラクタ
CollisionSphere::~CollisionSphere()
{
}

void CollisionSphere::Draw()
{
#ifdef _DEBUG
	// デバッグ用の当たり判定の可視化
	VECTOR centerPos = MyMath::VecAdd(*m_TargetPos, m_LocalPos);
	DrawSphere3D(centerPos, m_Radius, 16, GetColor(255, 255, 255), GetColor(255, 255, 255), false);
#endif
}

VECTOR CollisionSphere::GetWorldPos() const
{
	if (m_TargetPos) 
	{
		return MyMath::VecAdd(*m_TargetPos, m_LocalPos); 
	}
	return VGet(0.0f, 0.0f, 0.0f); 
}

bool CollisionSphere::CheckSphere(CollisionSphere* other)
{
	VECTOR centerPos = GetWorldPos();
	VECTOR otherCenterPos = other->GetWorldPos();

	VECTOR vec = MyMath::VecCreate(centerPos, otherCenterPos);
	float distance = MyMath::VecLong(vec);

	return distance <= (m_Radius + other->GetRadius());
}

bool CollisionSphere::CheckCapsule(class CollisionCapsule* capsule)
{
	// 球の中心位置
	VECTOR sphereCenter = GetWorldPos();
	// カプセルの両端点
	VECTOR capsuleStart = capsule->GetWorldPosStart();
	VECTOR capsuleEnd = capsule->GetWorldPosEnd();
	float capsuleRadius = capsule->GetRadius();
	// 球の中心からカプセルの線分への最短距離を計算
	float distance = MyMath::SegmentToSegmentDistance(
		sphereCenter, sphereCenter,
		capsuleStart, capsuleEnd
	);
	// 衝突判定
	return distance <= (m_Radius + capsuleRadius);
}


