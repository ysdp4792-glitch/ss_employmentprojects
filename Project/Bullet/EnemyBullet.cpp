#include "EnemyBullet.h"
#include "../Player/Player.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionSphere.h"
#include "../MyMath/MyMath.h"

EnemyBullet::EnemyBullet()
{
	m_Handle = -1;

	m_Pos = VGet(0, 0, 0);
	m_Rot = VGet(0, 0, 0);
	m_Scale = VGet(1, 1, 1);

	m_Dir = VGet(0, 0, 1);
	m_Speed = 0.3f;

	m_LifeTime = 5.0f;
	m_Timer = 0.0f;

	m_Target = nullptr;
	m_Sphere = nullptr;
}

EnemyBullet::~EnemyBullet()
{
	Fin();
}

void EnemyBullet::Init()
{
	m_IsDead = false;
	m_Timer = 0.0f;
}

void EnemyBullet::Load()
{
	m_Handle = MV1LoadModel("Data/Bullet/Bullet.x");
}

void EnemyBullet::Start()
{
	// 当たり判定
	m_Sphere = CollisionManager::GetInstance()->CreateSphere();
	m_Sphere->SetTargetPos(&m_Pos);
	m_Sphere->SetRadius(0.5f);
}

void EnemyBullet::Step()
{
	if (m_IsDead) return;

	// 移動
	m_Pos = VAdd(m_Pos, VScale(m_Dir, m_Speed));

	// 寿命
	m_Timer += 1.0f / 60.0f;
	if (m_Timer >= m_LifeTime)
	{
		m_IsDead = true;
	}
}

void EnemyBullet::Update()
{
	if (m_IsDead) return;

	MV1SetPosition(m_Handle, m_Pos);
	MV1SetRotationXYZ(m_Handle, m_Rot);
	MV1SetScale(m_Handle, m_Scale);
}

void EnemyBullet::Draw()
{
	if (!m_IsDead)
	{
		MV1DrawModel(m_Handle);
	}
}

void EnemyBullet::Fin()
{
	if (m_Handle >= 0)
	{
		MV1DeleteModel(m_Handle);
		m_Handle = -1;
	}
}

BulletBase* EnemyBullet::Clone()
{
	EnemyBullet* clone = new EnemyBullet;
	*clone = *this;
	clone->m_Handle = MV1DuplicateModel(m_Handle);
	return clone;
}

void EnemyBullet::SetPosition(const VECTOR& pos)
{
	m_Pos = pos;
}

void EnemyBullet::SetTarget(Player* player)
{
	m_Target = player;
}

void EnemyBullet::HitPlayer(Player* player)
{
	if (!player) return;

	player->Damage(1);
	m_IsDead = true;
}

void EnemyBullet::SetDirection(const VECTOR& dir)
{
	m_Dir = VNorm(dir);
	m_Rot.y = atan2f(m_Dir.x, m_Dir.z);
}
