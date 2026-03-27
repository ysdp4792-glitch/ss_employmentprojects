#pragma once

#include "PartsBase.h"

class CollisionAABB;

class PartsHeadBody : public PartsBase
{
public:
    PartsHeadBody();
    ~PartsHeadBody();

    void Init() override;
    void Load() override;
    void Start() override;
    void Step() override;
    void Update() override;
    void Draw() override;
    void Fin() override;

    CollisionCapsule* GetCapsule() override { return nullptr; }
    CollisionAABB* GetAABB() override { return m_AABB; }
    std::string GetDisplayName() const override { return "頭と胴体パーツ"; }

    PartsType GetPartsType() const override { return PLAYER_HEAD_AND_BODY; }

public:
    int m_Handle;
    VECTOR m_Pos;
    VECTOR m_Rot;
    VECTOR m_Scale;
    CollisionAABB* m_AABB;
    PartsBase* Clone() override;
};
