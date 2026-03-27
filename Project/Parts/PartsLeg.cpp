#include "PartsLeg.h"
#include "../Player/Player.h"
#include "../Collision/CollisionAABB.h"
#include "../Collision/CollisionManager.h" 

#define ROTATION_NUM 35.0f

PartsLeg::PartsLeg()
{
	m_Handle = 0;
	m_Pos = VGet(0.0f, 0.0f, 0.0f);
	m_Rot = VGet(0.0f, 0.0f, 0.0f);
	m_Scale = VGet(0.05f, 0.05f, 0.05f);
	m_AABB = nullptr;
}

PartsLeg::~PartsLeg()
{
	Fin();
}

void PartsLeg::Init()
{
	
}

void PartsLeg::Load()
{
	PartsBase::Load();
	m_Handle = MV1LoadModel("Data/Parts/Leg/Player_Leg.x");
}

void PartsLeg::Start()
{
	m_Pos = VGet(-50.0f, 1.0f, -100.0f);
	m_Rot = VGet(0.0f, 0.0f, 0.0f);
	m_Scale = VGet(0.03f, 0.03f, 0.03f);

	CollisionManager* collisionManager = CollisionManager::GetInstance();
	if (collisionManager)
	{
		// AABBの当たり判定を設定
		m_AABB = CollisionManager::GetInstance()->CreateAABB();
		m_AABB->SetTargetPos(&m_Pos);
		m_AABB->SetLocalPos(VGet(0.0f, 5.0f, -0.5f));
		m_AABB->SetSize(VGet(1.0f, 1.0f, 4.0f));
	}
}

void PartsLeg::Step()
{
}

void PartsLeg::Update()
{
	m_Rot.y += DX_PI_F / ROTATION_NUM;  // 1フレームごとに少しずつ回転

	// 3Dモデルの座標を設定する
	MV1SetPosition(m_Handle, m_Pos);
	// 3Dモデルの回転値を設定する
	MV1SetRotationXYZ(m_Handle, m_Rot);
	// 3Dモデルのスケールを設定する
	MV1SetScale(m_Handle, m_Scale);
}

void PartsLeg::Draw()
{
	MV1DrawModel(m_Handle);
}

void PartsLeg::Fin()
{
	MV1DeleteModel(m_Handle);
}

void PartsLeg::OnHitPlayer(Player* player)
{
	if (!player) return;

	// 衝突したので位置を1フレーム前に戻す
	player->SetPos(player->GetPrevPos());
}

PartsBase* PartsLeg::Clone()
{

	// クローン用のオブジェクトを生成 
	PartsLeg* clone = new PartsLeg; 

	// 自身の中身をクローンにコピー
	*clone = *this;

	// 画像はDuplicateする必要がある
	clone->m_Handle = MV1DuplicateModel(m_Handle);

	// 出来上がったクローンを返却
	return clone;
}
