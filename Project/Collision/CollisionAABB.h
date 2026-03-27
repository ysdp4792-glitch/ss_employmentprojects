#pragma once

#include "DxLib.h"

class CollisionCapsule;
class CollisionSphere;		// 追加: CollisionSphere クラスの前方宣言

class CollisionAABB
{
public:
	CollisionAABB();
	~CollisionAABB();

public:
	void Draw();			// 描画

public:
	void SetTargetPos(VECTOR* targetPos) { m_TargetPos = targetPos; }
	void SetLocalPos(VECTOR localPos) { m_LocalPos = localPos; }
	void SetSize(VECTOR size) { m_Size = size; }
	void SetWorldPos(const VECTOR& pos) { m_WorldPos = pos; }
	VECTOR GetTargetPos() { return *m_TargetPos; }
	VECTOR GetLocalPos() { return m_LocalPos; }
	VECTOR GetSize() { return m_Size; }

public:
	bool CheckAABB(CollisionAABB* other);
	bool CheckCapsule(CollisionCapsule* capsule);
	bool CheckSphere(CollisionSphere* sphere);		// 追加: CollisionSphereとの当たり判定
private:
	// 対象の座標
	VECTOR* m_TargetPos;
	// 対象の座標を原点としたローカル座標
	VECTOR m_LocalPos;
	// 縦横奥行き幅
	VECTOR m_Size;
	// ワールド座標
	VECTOR m_WorldPos;
};
