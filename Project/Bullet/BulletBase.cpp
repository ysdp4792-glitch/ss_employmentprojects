#include "BulletBase.h"
#include "../Collision/CollisionManager.h"

BulletBase::BulletBase()
{
	m_Handle = 0;
	m_Pos = VGet(0.0f, 0.0f, 0.0f);
	m_Rot = VGet(0.0f, 0.0f, 0.0f);
	m_Scale = VGet(0.0f, 0.0f, 0.0f);
	m_IsDead = false;
}

BulletBase::~BulletBase()
{
	Fin();
}

void BulletBase::Init()
{
}

void BulletBase::Load() 
{
}

void BulletBase::Start()
{
}


void BulletBase::Step()
{
}

void BulletBase::Update()
{
}

void BulletBase::Draw()
{
	MV1DrawModel(m_Handle);
}

void BulletBase::Fin()
{
}

void BulletBase::HitEnemy(EnemyBase* enemy)
{
	if (!enemy) return;

	enemy->Damage(m_AttackPower); // ★ HP を減らす
	SetIsDead(true);
}


bool BulletBase::IsDead()
{
	return m_IsDead;
}
