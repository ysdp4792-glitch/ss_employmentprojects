#pragma once

#include "DxLib.h"

class CollisionCapsule
{
public:
	CollisionCapsule();
	~CollisionCapsule();

public:
	void Draw();
public:
	void SetTargetPos(VECTOR* targetPos) { m_TargetPos = targetPos; }
	void SetLocalPos(VECTOR localPos) { m_LocalPos = localPos; }
	void SetLocalRot(float rotationY) { m_RotationY = rotationY; }
	void SetRadius(float radius) { m_Radius = radius; }

	VECTOR* GetTargetPos() { return m_TargetPos; }
	VECTOR GetLocalPos() { return m_LocalPos; }
	float GetLocalRot() { return m_RotationY; }
	VECTOR GetWorldPos();
	VECTOR GetStartPoint() const;
	VECTOR GetEndPoint() const;
	VECTOR GetWorldPosStart() const;
	VECTOR GetWorldPosEnd() const;
	float GetRadius() { return m_Radius; }
public:
	static bool IsHit(CollisionCapsule* a, CollisionCapsule* b);
	bool CheckCollisionSphere(VECTOR spherePos, float sphereRadius);
private:
	// 対象の座標
	VECTOR* m_TargetPos;
	// 対象の座標を原点としたローカル座標
	VECTOR m_LocalPos;
	// 球の半径
	float m_Radius;
	// Y軸回転(ラジアン)
	float m_RotationY;
	VECTOR m_CapsulePos1, m_CapsulePos2;
	MV1_COLL_RESULT_POLY_DIM m_HitPolyDim;
};