#include "BulletManager.h"
#include "PlayerBullet.h"
#include "EnemyBullet.h"
#include "../Player/PlayerManager.h"

BulletManager* BulletManager::m_Instance = nullptr;

BulletManager::BulletManager()
{
	for (int i = 0; i < BULLET_TYPE_MAX; i++)
	{
		m_OriginalBullet[i] = nullptr;
	}
}

BulletManager::~BulletManager()
{
	Fin();
}

void BulletManager::Init()
{
	m_OriginalBullet[PLAYER_BULLET] = new PlayerBullet;
	m_OriginalBullet[PLAYER_BULLET_2] = new PlayerBullet;
	m_OriginalBullet[ENEMY_BULLET] = new EnemyBullet;
}

void BulletManager::Load()
{
	for (int i = 0; i < BULLET_TYPE_MAX; i++)
	{
		m_OriginalBullet[i]->Load();
	}
}

void BulletManager::Start()
{
	for (auto _bulletBase : m_BulletList)
	{
		_bulletBase->Start();
	}
}

void BulletManager::Step()
{
	for (auto _bulletBase : m_BulletList)
	{
		_bulletBase->Step();
	}
}

void BulletManager::Update()
{
	auto it = m_BulletList.begin();
	while (it != m_BulletList.end())
	{
		BulletBase* bullet = *it;
		if (bullet->IsDead())
		{
			delete bullet;
			it = m_BulletList.erase(it);
		}
		else
		{
			bullet->Update();
			++it;
		}
	}
}

void BulletManager::Draw()
{ 
	for (auto bullet : m_BulletList) 
	{
		if (!bullet->IsDead()) 
			bullet->Draw(); 
	}
}

void BulletManager::Fin()
{
	for (auto _bulletBase : m_BulletList)
	{
		delete _bulletBase;
	}

	m_BulletList.clear();

	for (auto _bulletBase : m_OriginalBullet)
	{
		delete _bulletBase;
	}
}

BulletBase* BulletManager::CreateBullet(BulletType type)
{
	// タイプに合わせたエネミーをクローンで生成
	BulletBase* bullet = m_OriginalBullet[type]->Clone();

	// 生成したエネミーを管理用リストに追加
	m_BulletList.push_back(bullet);

	// 返却すれば生成した後にいろいろいじれる
	return bullet;
}

void BulletManager::FireBullet(VECTOR pos, BulletType type, BulletOffsetType offsetType, void* owner)
{
	BulletBase* base = BulletManager::GetInstance()->CreateBullet(type);
	if (!base) return;

	base->SetPosition(pos);

	// PlayerBullet か EnemyBullet かを判定して適切にキャスト
	if (PlayerBullet* playerBullet = dynamic_cast<PlayerBullet*>(base))
	{
		playerBullet->SetOffsetType(offsetType);
		// 所有者情報が必要な場合はここでセット
		// playerBullet->SetOwner(static_cast<Player*>(owner));
	}
	// EnemyBullet など他の弾種にも対応可能
	//else if (EnemyBullet* enemyBullet = dynamic_cast<EnemyBullet*>(base))
	{
		// 必要な処理（例：enemyBullet->SetTarget(player)）
	}

	base->Start();
}

