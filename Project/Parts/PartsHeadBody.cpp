#include "PartsHeadBody.h"
#include "../Collision/CollisionAABB.h"
#include "../Collision/CollisionManager.h" 

#define ROTATION_NUM 35.0f

PartsHeadBody::PartsHeadBody()
{
	m_Handle = 0;
	m_Pos = VGet(0.0f, 0.0f, 0.0f);
	m_Rot = VGet(0.0f, 0.0f, 0.0f);
	m_Scale = VGet(0.0f, 0.0f, 0.0f);
	m_AABB = nullptr;
}

PartsHeadBody::~PartsHeadBody()
{
	Fin();
}

void PartsHeadBody::Init()
{
}

void PartsHeadBody::Load()
{
	PartsBase::Load();
	m_Handle = MV1LoadModel("Data/Parts/HeadBody/Player_Face&Body.x");
}

void PartsHeadBody::Start()
{
	m_Pos = VGet(100.0f, 1.0f, 15.0f);
	m_Rot = VGet(0.0f, 0.0f, 0.0f);
	m_Scale = VGet(0.05f, 0.05f, 0.05f);

	CollisionManager* collisionManager = CollisionManager::GetInstance();
	if (collisionManager)
	{
		// AABBの当たり判定を設定
		m_AABB = CollisionManager::GetInstance()->CreateAABB();
		m_AABB->SetTargetPos(&m_Pos);
		m_AABB->SetLocalPos(VGet(0.0f, 0.5f, -1.0f));
		m_AABB->SetSize(VGet(1.0f, 1.0f, 1.0f));
	}
}

void PartsHeadBody::Step()
{
}

void PartsHeadBody::Update()
{
	m_Rot.y += DX_PI_F / ROTATION_NUM;  // 1フレームごとに少しずつ回転

	// 3Dモデルの座標を設定する
	MV1SetPosition(m_Handle, m_Pos);
	// 3Dモデルの回転値を設定する
	MV1SetRotationXYZ(m_Handle, m_Rot);
	// 3Dモデルのスケールを設定する
	MV1SetScale(m_Handle, m_Scale);
}

void PartsHeadBody::Draw()
{
	MV1DrawModel(m_Handle); 
}

void PartsHeadBody::Fin()
{
	MV1DeleteModel(m_Handle);
}

PartsBase* PartsHeadBody::Clone()
{
	// クローン用のオブジェクトを生成
	PartsHeadBody* clone = new PartsHeadBody;

	// 自身の中身をクローンにコピー
	*clone = *this;

	// 画像はDuplicateする必要がある
	clone->m_Handle = MV1DuplicateModel(m_Handle);

	// 出来上がったクローンを返却
	return clone;
}

