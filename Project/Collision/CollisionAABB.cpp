#include "DxLib.h"
#include "CollisionAABB.h"
#include "CollisionCapsule.h"
#include "../MyMath/MyMath.h"
#include "CollisionSphere.h"
#include <algorithm> // std::max, std::min を使うために追加

// コンストラクタ
CollisionAABB::CollisionAABB()
{
	m_TargetPos = nullptr;
	m_LocalPos = VGet(0.0f, 0.0f, 0.0f);
	m_Size = VGet(0.0f, 0.0f, 0.0f);
}

// デストラクタ
CollisionAABB::~CollisionAABB()
{
}

void CollisionAABB::Draw()
{

	// デバッグ用の当たり判定の可視化
	VECTOR posA, posB;
	VECTOR centerPos = MyMath::VecAdd(*m_TargetPos, m_LocalPos);

	posA.x = centerPos.x - m_Size.x * 0.5f;
	posA.y = centerPos.y - m_Size.y * 0.5f;
	posA.z = centerPos.z - m_Size.z * 0.5f;
	posB.x = centerPos.x + m_Size.x * 0.5f;
	posB.y = centerPos.y + m_Size.y * 0.5f;
	posB.z = centerPos.z + m_Size.z * 0.5f;

	DrawCube3D(posA, posB, GetColor(255, 255, 255), GetColor(255, 255, 255), false);
}

bool CollisionAABB::CheckAABB(CollisionAABB* other)
{
	VECTOR centerPos = MyMath::VecAdd(*m_TargetPos, m_LocalPos);
	VECTOR otherCenterPos = MyMath::VecAdd(other->GetTargetPos(), other->GetLocalPos());
	VECTOR otherSize = other->GetSize();

	// 自身のボックスの各面の値
	float left		= centerPos.x - m_Size.x * 0.6f;	// 左
	float right		= centerPos.x + m_Size.x * 0.6f;	// 右
	float top		= centerPos.y + m_Size.y * 0.6f;	// 上
	float bottom	= centerPos.y - m_Size.y * 0.6f;	// 下
	float front		= centerPos.z - m_Size.z * 0.6f;	// 手前
	float back		= centerPos.z + m_Size.z * 0.6f;	// 奥

	// 対象のボックスの各面の値
	float otherLeft		= otherCenterPos.x - otherSize.x * 0.5f;	// 左
	float otherRight	= otherCenterPos.x + otherSize.x * 0.5f;	// 右
	float otherTop		= otherCenterPos.y + otherSize.y * 0.5f;	// 上
	float otherBottom	= otherCenterPos.y - otherSize.y * 0.5f;	// 下
	float otherFront	= otherCenterPos.z - otherSize.z * 0.5f;	// 手前
	float otherBack		= otherCenterPos.z + otherSize.z * 0.5f;	// 奥

	// 各面が交差していれば当たっている
	if (left <= otherRight && right >= otherLeft &&
		bottom <= otherTop && top >= otherBottom &&
		front <= otherBack && back >= otherFront)
	{
		return true;
	}

	return false;
}

// 追加: CollisionSphereとの当たり判定
bool CollisionAABB::CheckSphere(CollisionSphere* sphere)
{
	if (!sphere || !m_TargetPos) return false;

	// AABBのワールド中心を常にターゲット位置 + ローカル位置で計算する
	VECTOR aabbCenter = MyMath::VecAdd(*m_TargetPos, m_LocalPos);
	VECTOR aabbHalf = VGet(m_Size.x * 0.5f, m_Size.y * 0.5f, m_Size.z * 0.5f);

	// 球のワールド座標と半径を取得
	VECTOR sphereCenter = sphere->GetWorldPos();
	float radius = sphere->GetRadius();

	// 最近接点を計算（各軸ごとにクランプ）
	VECTOR closest;
	closest.x = MyMath::Clamp(sphereCenter.x, aabbCenter.x - aabbHalf.x, aabbCenter.x + aabbHalf.x);
	closest.y = MyMath::Clamp(sphereCenter.y, aabbCenter.y - aabbHalf.y, aabbCenter.y + aabbHalf.y);
	closest.z = MyMath::Clamp(sphereCenter.z, aabbCenter.z - aabbHalf.z, aabbCenter.z + aabbHalf.z);

	// 距離の2乗を計算
	float dx = closest.x - sphereCenter.x;
	float dy = closest.y - sphereCenter.y;
	float dz = closest.z - sphereCenter.z;
	float distSq = dx * dx + dy * dy + dz * dz;

	return distSq <= (radius * radius);
}

bool CollisionAABB::CheckCapsule(CollisionCapsule* capsule)
{
	if (!m_TargetPos || !capsule || !capsule->GetTargetPos()) return false;

	VECTOR boxCenter = MyMath::VecAdd(*m_TargetPos, m_LocalPos);
	VECTOR capsulePos = MyMath::VecAdd(*capsule->GetTargetPos(), capsule->GetLocalPos());

	/*--- AABB範囲を取得--- */
	VECTOR boxMin = {
		boxCenter.x - m_Size.x * 0.5f,
		boxCenter.y - m_Size.y * 0.5f,
		boxCenter.z - m_Size.z * 0.5f
	};
	VECTOR boxMax = {
		boxCenter.x + m_Size.x * 0.5f,
		boxCenter.y + m_Size.y * 0.5f,
		boxCenter.z + m_Size.z * 0.5f
	};
	// --------------------
	 
	// カプセルの中心座標（今回は球体で近似）
	float capsuleRadius = capsule->GetRadius();

	// AABB と球（capsuleの中心）との距離チェック
	float sqDist = 0.0f;

	// 各軸で最近傍点との距離の2乗を加算
	if (capsulePos.x < boxMin.x) sqDist += (boxMin.x - capsulePos.x) * (boxMin.x - capsulePos.x);
	else if (capsulePos.x > boxMax.x) sqDist += (capsulePos.x - boxMax.x) * (capsulePos.x - boxMax.x);

	if (capsulePos.y < boxMin.y) sqDist += (boxMin.y - capsulePos.y) * (boxMin.y - capsulePos.y);
	else if (capsulePos.y > boxMax.y) sqDist += (capsulePos.y - boxMax.y) * (capsulePos.y - boxMax.y);

	if (capsulePos.z < boxMin.z) sqDist += (boxMin.z - capsulePos.z) * (boxMin.z - capsulePos.z);
	else if (capsulePos.z > boxMax.z) sqDist += (capsulePos.z - boxMax.z) * (capsulePos.z - boxMax.z);

	// 判定：球とAABBがぶつかっているか（距離の2乗と半径の2乗を比較）
	return sqDist <= (capsuleRadius * capsuleRadius);
}

