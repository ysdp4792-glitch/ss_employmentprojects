#pragma once

#include <vector>
// Box.hをインクルードしなくて済むように前方定義
class CollisionAABB;
class CollisionSphere;
class CollisionCapsule;

#define COLLISION_MAX 16

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

public:
	// マネージャーインスタンス管理
	static void CreateInstance() { if (!m_Instance) m_Instance = new CollisionManager; }
	// マネージャーの関数が呼びたいときに使用する、マネージャー取得関数
	static CollisionManager* GetInstance() { return m_Instance; }
	// 使わなくなったら削除する際の削除関数
	static void DeleteInstance() { if (m_Instance) delete m_Instance; m_Instance = nullptr; }

public:
	void Draw();	// 描画
	void Fin();		// 終了

public:
	// AABBを生成する
	CollisionAABB* CreateAABB();
	// AABBを削除する
	void DeleteAABB(CollisionAABB* targetAABB);
	// Sphereを生成する
	CollisionSphere* CreateSphere();
	// Sphereを削除する
	void DeleteSphere(CollisionSphere* targetSphere);
	// Capsuleを生成する
	CollisionCapsule* CreateCapsule();
	// Capsuleを削除する
	void DeleteCapsule(CollisionCapsule* targetCapsule);
public:
	// 当たり判定のチェック
	void CheckCollision();
private:
	// CollisionManagerインスタンス
	static CollisionManager* m_Instance;
	// 当たり判定管理用配列
	CollisionAABB* m_AABB[COLLISION_MAX];
	std::vector<CollisionSphere*> m_Sphere;
	CollisionCapsule* m_Capsule[COLLISION_MAX];
};
