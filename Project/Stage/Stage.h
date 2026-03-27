#pragma once

#include "DxLib.h"

class Stage 
{
public:
	Stage();
	~Stage();

public:
	void Load();
	void Start();
	void Draw();
	void Fin();

	MV1_COLL_RESULT_POLY_DIM GetHitResult() const { return m_CollisionResult; }

	bool CheckCollisionCapsule(VECTOR pos1,VECTOR pos2,float r);
	bool CheckCollisionSphere(VECTOR center, float radius);
private:
	int m_Handle;
	int m_CollisionHandle;
	MV1_COLL_RESULT_POLY_DIM m_CollisionResult;
};
