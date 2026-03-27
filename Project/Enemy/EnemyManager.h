#pragma once
#include "EnemyBase.h"
#include "NormalEnemy.h"
#include "BossEnemy.h"
#include <list>
#include <vector>

struct GameObject;

enum EnemyID
{
	NORMAL_ENEMY,
	BOSS_ENEMY,
	ENEMY_MAX,
};

// プレイヤーオブジェクト管理クラス
class EnemyManager
{
public:
	EnemyManager();	// コンストラクタ
	~EnemyManager();	// デストラクタ

	static void CreateInstance() { if (!m_Instance) m_Instance = new EnemyManager; }
	static EnemyManager* GetInstance() { return m_Instance; }
	static void DeleteInstance() { if (m_Instance) delete m_Instance; m_Instance = nullptr; }

	void Init();
	void Load();
	void LoadFromJson(const char* fileName);	// ロード
	void Start();	// 開始
	void Draw();	// 描画
	void Step();
	void Update();
	void Fin();		// 終了

	EnemyBase* CreateEnemy(int id);
	EnemyBase* CreateEnemy(int id, VECTOR pos, VECTOR rot, VECTOR scale);
	std::list<EnemyBase*>& GetEnemyList() { return m_EnemyList; }
private:
	static EnemyManager* m_Instance;

	EnemyBase* m_OriginalEnemy[ENEMY_MAX];

	std::list<EnemyBase*> m_EnemyList;

	std::vector<GameObject> m_Objects;
};
