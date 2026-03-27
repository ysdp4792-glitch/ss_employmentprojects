#pragma once
#include "PartsBase.h"
#include "../Collision/CollisionAABB.h"

class PartsLeftArm : public PartsBase
{
public:
    PartsLeftArm();
    ~PartsLeftArm();

    void Init() override;
    void Load() override;
    void Start() override;
    void Step() override;
    void Update() override;
    void Draw() override;
    void Fin() override;

    CollisionCapsule* GetCapsule() override { return nullptr; }
    CollisionAABB* GetAABB() override { return m_AABB; }
    std::string GetDisplayName() const override { return "左腕パーツ"; }

    PartsType GetPartsType() const override { return PLAYER_LEFT_ARM; }

    PartsBase* Clone() override;

private:
    int m_Handle;
    VECTOR m_Pos;
    VECTOR m_Rot;
    VECTOR m_Scale;
    CollisionAABB* m_AABB;
};
