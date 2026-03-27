#include <DxLib.h>
#include "PartsLeftArm.h"
#include "../Collision/CollisionCapsule.h"
#include "../Collision/CollisionManager.h"

#define ROTATION_NUM 35.0f

PartsLeftArm::PartsLeftArm()
{
    m_Handle = 0;
    m_Pos = VGet(0.0f, 0.0f, 0.0f);
    m_Rot = VGet(0.0f, 0.0f, 0.0f);
    m_Scale = VGet(0.1f, 0.1f, 0.1f);
    m_AABB = nullptr;
}

PartsLeftArm::~PartsLeftArm()
{
    Fin();
}

void PartsLeftArm::Init()
{
}

void PartsLeftArm::Load()
{
    PartsBase::Load(); 
    m_Handle = MV1LoadModel("Data/Parts/LeftArm/Player_LeftArm.x"); 
}

void PartsLeftArm::Start()
{
    m_Pos = VGet(-5.0f, 0.0f, -100.0f);
    m_Rot = VGet(0.0f, 0.0f, 0.0f);
    m_Scale = VGet(0.05f, 0.05f, 0.05f);

    CollisionManager* collisionManager = CollisionManager::GetInstance();
    if (collisionManager)
    {
        // AABBの当たり判定生成
        m_AABB = collisionManager->CreateAABB();
        if (m_AABB)
        {
            m_AABB->SetTargetPos(&m_Pos);
            m_AABB->SetLocalPos(VGet(0.0f, 0.0f, 0.0f));
            m_AABB->SetSize(VGet(1.0f, 1.0f, 1.0f)); // サイズを設定
        }
    }
}

void PartsLeftArm::Step()
{
    // 将来のアニメーション更新や制御用
}

void PartsLeftArm::Update()
{
    m_Rot.y += DX_PI_F / ROTATION_NUM;  // 1フレームごとに少しずつ回転

    if (m_Handle != 0)
    {
        MV1SetPosition(m_Handle, m_Pos);
        MV1SetRotationXYZ(m_Handle, m_Rot);
        MV1SetScale(m_Handle, m_Scale);
    }
}

void PartsLeftArm::Draw()
{
    MV1DrawModel(m_Handle);
}

void PartsLeftArm::Fin()
{
    if (m_Handle != 0)
    {
        MV1DeleteModel(m_Handle);
        m_Handle = 0;
    }

    if (m_AABB)
    {
        CollisionManager::GetInstance()->DeleteAABB(m_AABB);
        m_AABB = nullptr;
    }
}

PartsBase* PartsLeftArm::Clone()
{
    PartsLeftArm* clone = new PartsLeftArm(*this); // コピーコンストラクタで値コピー
    clone->m_Handle = MV1DuplicateModel(m_Handle);  // モデル複製
    clone->m_AABB = nullptr;                     // Capsuleは再生成が必要
    return clone;
}
