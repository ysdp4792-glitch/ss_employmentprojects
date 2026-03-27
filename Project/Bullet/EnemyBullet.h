#pragma once
#include "BulletBase.h"
#include <DxLib.h>

class Player;
class CollisionSphere;

class EnemyBullet : public BulletBase
{
public:
	EnemyBullet();
	~EnemyBullet();

	void Init() override;
	void Load() override;
	void Start() override;
	void Step() override;
	void Update() override;
	void Draw() override;
	void Fin() override;

	BulletBase* Clone() override;

	void SetPosition(const VECTOR& pos);
	void SetTarget(Player* player);
	void HitPlayer(Player* player);
	void SetDirection(const VECTOR& dir);

private:
	VECTOR m_Dir;
	float  m_Speed;

	float  m_LifeTime;
	float  m_Timer;

	Player* m_Target;
	CollisionSphere* m_Sphere;
};
