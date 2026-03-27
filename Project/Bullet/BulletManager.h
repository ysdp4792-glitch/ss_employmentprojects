#pragma once
#include <list>
#include "BulletBase.h"
#include "PlayerBullet.h"
#include "EnemyBullet.h"

class PlayerBullet;

enum BulletType
{
	PLAYER_BULLET,
	PLAYER_BULLET_2,
	ENEMY_BULLET,
	BULLET_TYPE_MAX,
	BULLET_TYPE_NONE = -1
};

class BulletManager
{
public:
	BulletManager();
	~BulletManager();

public:
	static void CreateInstance() { if (!m_Instance) m_Instance = new BulletManager; }
	static BulletManager* GetInstance() { return m_Instance; }
	static void DeleteInstance() { if (m_Instance)delete m_Instance; m_Instance = nullptr; }

public:
	void Init();
	void Load();
	void Start();
	void Step();
	void Update();
	void Draw();
	void Fin();

public:
	BulletBase* CreateBullet(BulletType type);
	std::list<BulletBase*> GetPlayerBullet() { return m_BulletList; }
	void FireBullet(VECTOR pos, BulletType type, BulletOffsetType offsetType, void* owner); 

private:

	static BulletManager* m_Instance;

	BulletBase* m_OriginalBullet[BULLET_TYPE_MAX]; 

	std::list<BulletBase*> m_BulletList;
};
