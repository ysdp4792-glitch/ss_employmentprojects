#include "CollisionManager.h"
#include "CollisionAABB.h"
#include "CollisionSphere.h"
#include "CollisionCapsule.h"
#include "../Player/PlayerManager.h"
#include "../Player/Player.h"
#include "../Stage/Stage.h"
#include "../Stage/StageManager.h"
#include "../Bullet/BulletBase.h"
#include "../Bullet/BulletManager.h"
#include "../Parts/PartsManager.h"
#include "../Enemy/EnemyManager.h"

// 静的変数の初期化
CollisionManager* CollisionManager::m_Instance = nullptr;

CollisionManager::CollisionManager()
{
    for (int i = 0; i < COLLISION_MAX; i++)
    {
        m_AABB[i] = nullptr;
        m_Capsule[i] = nullptr;
    }
    m_Sphere.clear();
}

CollisionManager::~CollisionManager()
{
    Fin();
}

void CollisionManager::Draw()
{
    for (auto aabb : m_AABB)
    {
        if (aabb)
        {
            aabb->Draw();
        }
    }

    for (auto sphere : m_Sphere)
    {
        if (sphere)
        {
            sphere->Draw();
        }
    }

    for (auto capsule : m_Capsule)
    {
        if (capsule)
        {
            capsule->Draw();
        }
    }
}

void CollisionManager::Fin()
{
    for (int i = 0; i < COLLISION_MAX; i++)
    {
        if (m_AABB[i])
        {
            delete m_AABB[i];
            m_AABB[i] = nullptr;
        }
        if (i < (int)m_Sphere.size() && m_Sphere[i])
        {
            delete m_Sphere[i];
            m_Sphere[i] = nullptr;
        }
        if (m_Capsule[i])
        {
            delete m_Capsule[i];
            m_Capsule[i] = nullptr;
        }
    }
    m_Sphere.clear();
}

CollisionAABB* CollisionManager::CreateAABB()
{
    CollisionAABB* result = nullptr;
    for (int i = 0; i < COLLISION_MAX; i++)
    {
        if (!m_AABB[i])
        {
            m_AABB[i] = result = new CollisionAABB;
            break;
        }
    }
    return result;
}

void CollisionManager::DeleteAABB(CollisionAABB* targetAABB)
{
    for (int i = 0; i < COLLISION_MAX; i++)
    {
        if (m_AABB[i] == targetAABB)
        {
            delete targetAABB;
            m_AABB[i] = nullptr;
            break;
        }
    }
}

CollisionSphere* CollisionManager::CreateSphere()
{
    CollisionSphere* result = new CollisionSphere;
    m_Sphere.push_back(result);
    return result;
}

void CollisionManager::DeleteSphere(CollisionSphere* targetSphere)
{
    for (int i = 0; i < (int)m_Sphere.size(); i++)
    {
        if (m_Sphere[i] == targetSphere)
        {
            delete targetSphere;
            m_Sphere[i] = nullptr;
            m_Sphere.erase(m_Sphere.begin() + i);
            break;
        }
    }
}

CollisionCapsule* CollisionManager::CreateCapsule()
{
    CollisionCapsule* result = nullptr;
    for (int i = 0; i < COLLISION_MAX; i++)
    {
        if (!m_Capsule[i])
        {
            m_Capsule[i] = result = new CollisionCapsule;
            break;
        }
    }
    return result;
}

void CollisionManager::DeleteCapsule(CollisionCapsule* targetCapsule)
{
    for (int i = 0; i < COLLISION_MAX; i++)
    {
        if (m_Capsule[i] == targetCapsule)
        {
            delete targetCapsule;
            m_Capsule[i] = nullptr;
            break;
        }
    }
}

// 当たり判定の処理
void CollisionManager::CheckCollision()
{
    Player* player = PlayerManager::GetInstance()->GetPlayer();
    Stage* stage = StageManager::GetInstance()->GetStage();
    CollisionCapsule* playerCapsule = player->GetCapsuleCollision();
    std::list<BulletBase*> bulletList = BulletManager::GetInstance()->GetPlayerBullet();

    // プレイヤーとステージの当たり判定
    if (stage->CheckCollisionCapsule(playerCapsule->GetStartPoint(), playerCapsule->GetEndPoint(), playerCapsule->GetRadius()))
    {
        player->HitStage(stage);
    }

    // 弾とステージの当たり判定
    for (BulletBase* bullet : bulletList)
    {
        if (!bullet || bullet->IsDead()) continue;

        CollisionSphere* sphere = bullet->GetCollisionSphere();
        if (!sphere) continue;

        if (stage->CheckCollisionSphere(sphere->GetWorldPos(), sphere->GetRadius()))
        {
            bullet->SetIsDead(true);
        }
    }

    // ここで「弾」と「プレイヤー」の当たり判定を行う（敵弾がプレイヤーに当たる処理）
    if (player)
    {
		CollisionCapsule* playerCapsule = player->GetCapsuleCollision();
        for (BulletBase* bullet : bulletList)
        {
            if (!bullet || bullet->IsDead()) continue;

            CollisionSphere* sphere = bullet->GetCollisionSphere();
            if (!sphere) continue;

            // AABB を使った球判定でプレイヤーに当たったか確認
            if (playerCapsule && sphere && playerCapsule->CheckCollisionSphere(sphere->GetWorldPos(), sphere->GetRadius()))
            {
                // ポリモーフィズムで弾種ごとの反応（EnemyBullet::HitPlayer が呼ばれる）
                bullet->HitPlayer(player);
                // 多くの弾は命中で消えるので念のためフラグを立てる
                bullet->SetIsDead(true);
            }
        }
    }

    auto& enemyList = EnemyManager::GetInstance()->GetEnemyList();

    for (BulletBase* bullet : bulletList)
    {
        if (!bullet || bullet->IsDead()) continue;

        CollisionSphere* sphere = bullet->GetCollisionSphere();
        if (!sphere) continue;

        for (EnemyBase* enemy : enemyList)
        {
            if (!enemy || enemy->IsDead()) continue;

            CollisionAABB* enemyAABB = enemy->GetAABB();
            if (!enemyAABB) continue;

            if (enemyAABB->CheckSphere(sphere))
            {
                // ★ ここで弾側に処理を任せる
                bullet->HitEnemy(enemy);
                break; // 1発で1体だけ当てる
            }

            // 敵の代表位置（中心）を使った球判定（簡易）
       // ここで使用する半径は敵ごとに持たせても良い（仮：1.0f）
            const float enemyCheckRadius = 1.0f;

            if (stage->CheckCollisionSphere(enemy->GetPos(), enemyCheckRadius))
            {
                // 衝突情報は Stage に保持されている（CheckCollisionSphere 実行で更新済み）
                // 敵側で高さ補正等の応答を行う
                enemy->HitStage(stage);
            }
        }
    }


    auto& partsList = PartsManager::GetInstance()->GetPartsList();
    for (auto it = partsList.begin(); it != partsList.end(); )
    {
        PartsBase* part = *it;
        if (!part) {
            ++it;
            continue;
        }

        bool hit = false;

        CollisionCapsule* partCapsule = part->GetCapsule();
        if (partCapsule && CollisionCapsule::IsHit(playerCapsule, partCapsule))
        {
            hit = true;
        }
        else
        {
            CollisionAABB* partAABB = part->GetAABB();
            if (partAABB && partAABB->CheckCapsule(playerCapsule))
            {
                hit = true;
            }
        }

        if (hit)
        {
            player->AddPart(part);        // プレイヤーに追加
            part->OnHitPlayer(player);    // 表示などの処理

            // PartsManager 経由で削除
            PartsManager::GetInstance()->RemovePart(part);
            ++it;
        }
        else
        {
            ++it;
        }

    }
}
