#pragma once

#include "PartsBase.h"

class PartsBase;
class CollisionCapsule;
class CollisionAABB;

class PartsRightArm : public PartsBase
{
public:
    PartsRightArm();
    ~PartsRightArm();
public:
    void Init()override;    // 初期化
    void Load()override;    // ロード
    void Start()override;   // 開始
    void Step()override;
    void Update()override;  // 更新
    void Draw()override;    // 描画
    void Fin()override;     // 終了
    CollisionCapsule* GetCapsule()override { return nullptr; }
    CollisionAABB* GetAABB()override { return m_AABB; }
    std::string GetDisplayName() const override { return "右腕パーツ"; }

    PartsType GetPartsType() const override { return PLAYER_RIGHT_ARM; } 

public:
    int m_Handle;
    VECTOR m_Pos;   // 座標
    VECTOR m_Rot;   // 回転
    VECTOR m_Scale; // スケール
    CollisionCapsule* m_Capsule;
    CollisionAABB* m_AABB;
    PartsBase* Clone()override;
};
