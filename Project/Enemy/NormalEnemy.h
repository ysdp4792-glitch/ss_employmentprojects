#pragma once
#include <DxLib.h>
#include "EnemyBase.h"
#include "../Collision/CollisionAABB.h"
#define MUZZLE_NUM 8
class Player;

class NormalEnemy : public EnemyBase
{
public:
    NormalEnemy();
    ~NormalEnemy();

    void Init() override;
    void Load() override;
    void Start() override;
    void Update() override;
    void Draw() override;
    void Step() override; // 追加: EnemyBaseの純粋仮想関数Step()の実装宣言
    EnemyBase* Clone() override; // 追加: EnemyBaseの純粋仮想関数Clone()の実装宣言

    void DrawDetectArea();

    bool DetectPlayer(Player* player);

    void PatrolMove();

    void ChasePlayer(Player* player);

    void Shoot();

    void GetMuzzleWorldPos(VECTOR& outPos);

    static VECTOR VMult(const VECTOR& v1, const VECTOR& v2);

private:
    
    enum State
    {
        Patrol,
        Chase
    };
    State m_State;
    float m_LostTimer;
    int m_CurrentMuzzle;
    float m_ShootTimer;

    // --- 追加: プレイヤー検知範囲 ---
    float m_DetectRange = 15.0f; // 適切な初期値を設定

    // パトロール用
    int m_PatrolTimer = 0;
    VECTOR m_PatrolDir = VGet(0, 0, 1);

    // マズルボーン
    int m_MuzzleBone[MUZZLE_NUM];
};

