#include "PartsRightArm.h"
#include "../Collision/CollisionAABB.h"
#include "../Collision/CollisionManager.h" 

#define ROTATION_NUM 35.0f

PartsRightArm::PartsRightArm()
{
		m_Handle = 0;
		m_Pos = VGet(0.0f, 0.0f, 0.0f);
		m_Rot = VGet(0.0f, 0.0f, 0.0f);
		m_Scale = VGet(0.0f, 0.0f, 0.0f);
		m_Capsule = nullptr;
}

PartsRightArm::~PartsRightArm()
{
	Fin();
}

void PartsRightArm::Init()
{
}

void PartsRightArm::Load()
{
	PartsBase::Load();
	m_Handle = MV1LoadModel("Data/Parts/RightArm/Player_RightArm.x");
}


void PartsRightArm::Start()
{
	m_Pos = VGet(5.0f, 1.0f, -15.0f);
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

void PartsRightArm::Step()
{

}

void PartsRightArm::Update()
{
	m_Rot.y += DX_PI_F / ROTATION_NUM;

		MV1SetPosition(m_Handle, m_Pos);
		MV1SetRotationXYZ(m_Handle, m_Rot);
		MV1SetScale(m_Handle, m_Scale);
}


void PartsRightArm::Draw()
{
	MV1DrawModel(m_Handle);
}

void PartsRightArm::Fin()
{
	MV1DeleteModel(m_Handle);
}


PartsBase* PartsRightArm::Clone()
{
	// クローン用のオブジェクトを生成
	PartsRightArm* clone = new PartsRightArm;

	// 自身の中身をクローンにコピー
	*clone = *this;

	// 画像はDuplicateする必要がある
	clone->m_Handle = MV1DuplicateModel(m_Handle);

	// 出来上がったクローンを返却
	return clone;
}
