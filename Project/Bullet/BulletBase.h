#pragma once

#include <DxLib.h>

#include "../Enemy/EnemyBase.h"

class CollisionSphere;
class EnemyBase;
class Player; // 追加: プレイヤー型の前方宣言

class BulletBase
{
public:
	BulletBase();
	~BulletBase();

public:
	virtual void Init();
	virtual void Load();	// ロード
	virtual void Start();	// 開始
	virtual void Step();	// ステップ
	virtual void Update();	// 更新
	virtual void Draw();	// 描画
	virtual void Fin();		// 終了
	virtual void HitEnemy(EnemyBase* enemy);
	// プレイヤーに当たった時のコールバック（デフォルトは何もしない）
	virtual void HitPlayer(Player* player) {}
	virtual bool IsDead();
	virtual BulletBase* Clone() = 0;
public:
	CollisionSphere* GetCollisionSphere() { return m_Sphere; }
	// BulletBase.h
	virtual void SetPosition(const VECTOR& pos) = 0;
	void SetIsDead(bool isDead) { m_IsDead = isDead; }
protected:
	int m_Handle;
	bool m_IsDead = false;
	VECTOR m_Pos;	// 座標
	VECTOR m_Rot;	// 回転
	VECTOR m_Scale;	// スケール
	CollisionSphere* m_Sphere;
	int m_AttackPower = 10; // デフォルト値は適宜調整
};
