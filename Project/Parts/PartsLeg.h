#pragma once

#include "PartsBase.h"
#include <string>

class PartsBase;
class CollisionAABB;

class PartsLeg : public PartsBase
{
public:
	PartsLeg();
	~PartsLeg();
public:
	void Init()override;	// 初期化
	void Load()override;	// ロード
	void Start()override;	// 開始
	void Step()override;
	void Update()override;	// 更新
	void Draw()override;	// 描画
	void Fin()override;		// 終了
	CollisionAABB* GetAABB()override { return m_AABB; }
	CollisionCapsule* GetCapsule()override { return nullptr; }
	void  OnHitPlayer(Player* player) override;
	PartsType GetPartsType() const override { return PLAYER_LEG; }
	std::string GetDisplayName()const override { return "足のパーツ"; }
private:
	int m_Handle;
	VECTOR m_Pos;	// 座標
	VECTOR m_Rot;	// 回転
	VECTOR m_Scale;	// スケール
	CollisionAABB* m_AABB;
	PartsBase* Clone()override;
};
