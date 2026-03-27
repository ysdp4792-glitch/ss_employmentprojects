#pragma once
#include <DxLib.h>
#include "../Collision/CollisionAABB.h"
#include <string>

class CollisionAABB;

class Stage;

class EnemyBase
{
public:
	EnemyBase();
	~EnemyBase();
public:
	virtual void Init() = 0;
	virtual void Load() = 0;
	virtual void Start() = 0;
	virtual void Step() = 0;

	virtual void Update();
	virtual void Draw();
	virtual void Fin();

	virtual EnemyBase* Clone() = 0;

	void Damage(int damage);
	void HitStage(Stage* stage);
	bool IsDead() const { return m_IsDead; };
	CollisionAABB* GetAABB() { return m_AABB; }

public:
	// Transform 設定 — ここで AABB を更新する
	void SetTransform(VECTOR pos, VECTOR rot, VECTOR scale)
	{
		m_Pos = pos;
		m_Rot = rot;
		m_Scale = scale;
		UpdateCollisionFromTransform();
	}
	void SetPos(VECTOR pos) { m_Pos = pos; }
	VECTOR GetPos() const { return m_Pos; }

	// 共通 HUD / 属性
	void SetName(const std::string& name) { m_Name = name; }
	void SetMaxHP(int maxHp) { m_MaxHP = maxHp; if (m_HP > m_MaxHP) m_HP = m_MaxHP; }
	int GetMaxHP() const { return m_MaxHP; }
	int GetHP() const { return m_HP; }
	void SetHP(int hp) { m_HP = hp; if (m_HP < 0) m_HP = 0; if (m_HP > m_MaxHP) m_HP = m_MaxHP; }

	void DrawHUD();

protected:
	// AABB の基準サイズを返す（敵ごとに override して基準サイズを提供）
	virtual VECTOR GetAABBBaseSize() const { return VGet(0.0f, 0.0f, 0.0f); }

	// Transform 更新時に AABB を更新する（内部ヘルパ）
	void UpdateCollisionFromTransform()
	{
		if (m_AABB)
		{
			VECTOR base = GetAABBBaseSize();
			// base がゼロなら更新しない
			if (!(base.x == 0.0f && base.y == 0.0f && base.z == 0.0f))
			{
				// スケールごとに拡縮（コンポーネント毎に掛ける）
				VECTOR newSize = VGet(base.x * m_Scale.x, base.y * m_Scale.y, base.z * m_Scale.z);
				m_AABB->SetSize(newSize);
				// 必要ならローカルオフセットも調整する実装をここに追加可能
			}
		}
	}

protected:
	int m_Handle = -1;
	int m_HP = 100;
	int m_MaxHP = 100;
	std::string m_Name;
	VECTOR m_Pos = VGet(0, 0, 0);
	VECTOR m_Rot = VGet(0, 0, 0);
	VECTOR m_Scale = VGet(1, 1, 1); // デフォルトを 0 -> 1 に変更（重要）
	VECTOR m_Move = VGet(0, 0, 0);
	CollisionAABB* m_AABB = nullptr;
	bool m_IsDead = false;
};