#include "EnemyManager.h"
#include "EnemyParaMeter.h"
#include "NormalEnemy.h"
#include "BossEnemy.h"
#include "../Scene/SceneManager.h"
#include <fstream>

EnemyManager* EnemyManager::m_Instance = nullptr;

// usingして使いやすくする
using json = nlohmann::json;
// データがまとまっている階層のキー名
constexpr const char* KEY_ITEMS = "items";

EnemyManager::EnemyManager()
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		m_OriginalEnemy[i] = nullptr;
	}
}

EnemyManager::~EnemyManager()
{
	Fin();
}

void EnemyManager::LoadFromJson(const char* fileName)
{
	// jsonファイルを開く
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		return;
	}
	// jsonデータをパース
	json jsonData;
	file >> jsonData;
	// オブジェクトデータを読み込む
	m_Objects = jsonData[KEY_ITEMS].get<std::vector<GameObject>>();
	file.close();
}

void EnemyManager::Init()
{
	// クローン元のエネミーを生成する
	m_OriginalEnemy[NORMAL_ENEMY] = new NormalEnemy;
	// ボスの雛形を追加
	m_OriginalEnemy[BOSS_ENEMY] = new BossEnemy;
}

void EnemyManager::Load()
{
	LoadFromJson("Data/Enemy/Enemy01.json");
	// クローン元をロードする（null チェックを追加）
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (m_OriginalEnemy[i]) {
			m_OriginalEnemy[i]->Load();
		}
	}
}

void EnemyManager::Start()
{
	// JSON に配置情報があればそれを元に生成
	if (!m_Objects.empty())
	{
		for (const auto& obj : m_Objects)
		{
			// JSON の id->EnemyID マッピング（必要に応じて調整してください）
			// ここでは obj.id == 1 を BOSS と仮定、その他は NORMAL とする
			int enemyType = (obj.id == 1) ? BOSS_ENEMY : NORMAL_ENEMY;

			EnemyBase* e = CreateEnemy(enemyType, obj.pos, obj.rot, obj.scale);
			if (e)
			{
				e->Start();
			}
		}
	}
	else
	{
		// フォールバック：従来どおり Normal を1体生成
		VECTOR pos = VGet(5.0f, 1.0f, 5.0f);
		VECTOR rot = VGet(0.0f, 0.0f, 0.0f);
		VECTOR scale = VGet(5.0f, 5.0f, 5.0f);

		EnemyBase* e = CreateEnemy(NORMAL_ENEMY, pos, rot, scale);
		if (e)
		{
			e->Start();
		}

		// 例：ボスを手動で出したい場合はここで生成（位置/スケールは調整してください）
		VECTOR bossPos = VGet(0.0f, 1.0f, 0.0f);
		VECTOR bossRot = VGet(0.0f, 0.0f, 0.0f);
		VECTOR bossScale = VGet(8.0f, 8.0f, 8.0f);

		EnemyBase* boss = CreateEnemy(BOSS_ENEMY, bossPos, bossRot, bossScale);
		if (boss)
		{
			boss->Start();
		}
	}
}

void EnemyManager::Step()
{
	// 範囲for文で安全にリストを回せる
	for (auto enemy : m_EnemyList)
	{
		enemy->Step();
	}
}

void EnemyManager::Update()
{
	// 範囲for文で安全にリストを回せる
	for (auto enemy : m_EnemyList)
	{
		enemy->Update();
	}
	// 死んだエネミーをリストから削除する
	for (auto it = m_EnemyList.begin(); it != m_EnemyList.end(); )
	{
		EnemyBase* current = *it;
		if (current->IsDead())
		{
			// NormalEnemy を倒したらボスがまだ存在しなければ出現させる
			if (dynamic_cast<NormalEnemy*>(current))
			{
				bool bossExists = false;
				for (auto e : m_EnemyList)
				{
					if (dynamic_cast<BossEnemy*>(e))
					{
						bossExists = true;
						break;
					}
				}
				// ボスが居なければ生成（倒れた敵の位置に出す）
				if (!bossExists && m_OriginalEnemy[BOSS_ENEMY])
				{
					VECTOR spawnPos = current->GetPos();
					VECTOR spawnRot = VGet(0.0f, 0.0f, 0.0f);
					VECTOR spawnScale = VGet(8.0f, 8.0f, 8.0f);
					EnemyBase* boss = CreateEnemy(BOSS_ENEMY, spawnPos, spawnRot, spawnScale);
					if (boss)
					{
						boss->Start();
					}
				}
			}

			// Boss が倒れたら即座にクリアシーンへ遷移
			if (dynamic_cast<BossEnemy*>(current))
			{
				delete current;
				it = m_EnemyList.erase(it);
				if (SceneManager::GetInstance())
				{
					SceneManager::GetInstance()->ChangeScene(CLEAR);
				}
				// もうこのイテレーションは終わり（シーン遷移した）, 続行は不要
				continue;
			}

			// 通常の削除処理
			delete current;
			it = m_EnemyList.erase(it);
		}
		else
		{
			++it;
		}
	}

	// 全ての敵が消えたらクリアシーンへ遷移（念のための保険）
	if (m_EnemyList.empty())
	{
		if (SceneManager::GetInstance())
		{
			SceneManager::GetInstance()->ChangeScene(CLEAR);
		}
	}
}

void EnemyManager::Draw()
{
	// 範囲for文で安全にリストを回せる
	for (auto enemy : m_EnemyList)
	{
		enemy->Draw();
	}
}

void EnemyManager::Fin()
{
	// 範囲for文で安全にリストを回せる
	for (auto enemy : m_EnemyList)
	{
		delete enemy;
	}

	// リストをクリア
	m_EnemyList.clear();

	// クローン元も削除する
	for (auto enemy : m_OriginalEnemy)
	{
		delete enemy;
	}
}

EnemyBase* EnemyManager::CreateEnemy(int id)
{
	// 安全チェックを追加：雛形が無ければ nullptr を返す
	if (id < 0 || id >= ENEMY_MAX) return nullptr;
	if (!m_OriginalEnemy[id]) return nullptr;

	// タイプに合わせたエネミーをクローンで生成
	EnemyBase* enemy = m_OriginalEnemy[id]->Clone();

	// 生成したエネミーを管理用リストに追加
	m_EnemyList.push_back(enemy);

	// 返却すれば生成した後にいろいろいじれる
	return enemy;
}

EnemyBase* EnemyManager::CreateEnemy(int id, VECTOR pos, VECTOR rot, VECTOR scale)
{
	EnemyBase* enemy = CreateEnemy(id);
	if (enemy)
	{
		enemy->SetTransform(pos, rot, scale);
	}
	return enemy;
}
