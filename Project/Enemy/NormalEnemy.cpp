#include <DxLib.h>
#include "NormalEnemy.h"
#include "../Player/PlayerManager.h"
#include "../Player/Player.h"
#include "../Bullet/BulletManager.h"
#include "../Bullet/EnemyBullet.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionAABB.h"
#include "../MyMath/MyMath.h"
#include "EnemyBase.h"

NormalEnemy::NormalEnemy()
{
	m_AABB = nullptr;
}

NormalEnemy::~NormalEnemy() {}

void NormalEnemy::Init()
{
	SetName("NormalEnemy");
	SetMaxHP(100);
	SetHP(GetMaxHP());

	m_State = Patrol;
	m_LostTimer = 0.0f;

	m_CurrentMuzzle = 0;
	m_ShootTimer = 0.0f;

	// パトロール初期化
	m_PatrolTimer = 120;
	m_PatrolDir = VGet(0.0f, 0.0f, 1.0f);

	// 追跡初距離（必要に応じで変更）
	m_DetectRange = 15.0f;
}

void NormalEnemy::Load()
{
	// 描画用モデルのみロード（コリジョン用モデルは使わない）
	m_Handle = MV1LoadModel("Data/Enemy/NormalEnemy/NormalEnemy.x");

	const char* muzzleName[MUZZLE_NUM] =
	{
		"MuzzleBorn_1","MuzzleBorn_2","MuzzleBorn_3","MuzzleBorn_4",
		"MuzzleBorn_5","MuzzleBorn_6","MuzzleBorn_7","MuzzleBorn_8"
	};

	for (int i = 0; i < MUZZLE_NUM; i++)
	{
		m_MuzzleBone[i] = MV1SearchFrame(m_Handle, muzzleName[i]);
	}

	// AABB を作成して敵の m_Pos に追従させる（当たり判定は AABB のみ）
	// ローカルオフセット・サイズは任意調整可
	m_AABB = CollisionManager::GetInstance()->CreateAABB();
	if (m_AABB)
	{
		m_AABB->SetTargetPos(&m_Pos);
		// AABB の中心を敵の足元から上方向へオフセット（高さ / 2 程度）
		m_AABB->SetLocalPos(VGet(0.0f, 1.0f, 0.0f));
		// AABB のサイズ（幅, 高さ, 奥行） — 必要に応じて値を調整してください
		m_AABB->SetSize(VGet(2.0f, 4.0f, 2.0f));
	}
}

void NormalEnemy::Start()
{
	m_Scale = VGet(10.0f, 10.0f, 10.0f);
}

void NormalEnemy::Step()
{
}

void NormalEnemy::Update()
{
	Player* player = PlayerManager::GetInstance()->GetControlledPlayer();
	if (!player) return;

	bool detected = DetectPlayer(player);

	// 見た目
	MV1SetPosition(m_Handle, m_Pos);
	MV1SetRotationXYZ(m_Handle, m_Rot);
	MV1SetScale(m_Handle, m_Scale);

	switch (m_State)
	{
	case Patrol:
		// 移動は行わずその場で索敵・回転のみ行う
		PatrolMove();
		if (detected)
		{
			m_State = Chase;
			m_LostTimer = 2.0f;
		}
		break;

	case Chase:
		// 追跡時も位置は変えず、その場でプレイヤー方向を向くだけにする
		ChasePlayer(player);
		if (detected)
		{
			m_LostTimer = 2.0f;
		}
		else
		{
			m_LostTimer -= 1.0f / 60.0f;
			if (m_LostTimer <= 0.0f)
			{
				m_State = Patrol;
			}
		}
		// ---- 射撃 ----
		m_ShootTimer -= 1.0f / 60.0f;
		if (m_ShootTimer <= 0.0f)
		{
			Shoot();
			m_ShootTimer = 0.08f; // ガトリング間隔
		}
		break;
	}
}

void NormalEnemy::Draw()
{
	MV1DrawModel(m_Handle);
	DrawDetectArea();
	DrawHUD();
	// ===== デバッグ：マズル位置 =====
	VECTOR muzzlePos;
	GetMuzzleWorldPos(muzzlePos);

	DrawSphere3D(
		muzzlePos,          // 中心座標
		0.2f,               // 半径（見やすい大きさ）
		8,                  // 分割数
		GetColor(255, 0, 0),// 赤
		GetColor(255, 0, 0),
		TRUE                // 塗りつぶし
	);
}

// 索敵範囲 
 void NormalEnemy::DrawDetectArea() 
 { 
	 float detectRange = m_DetectRange; 
	 int color = GetColor(0, 255, 0); int segment = 64; 
	 // 円の分割数（滑らかさ） 
	 VECTOR prev = VAdd(m_Pos, VGet(detectRange, 0, 0)); 
	 // 最初の点 
	 for (int i = 1; i <= segment; i++) 
	 { 
		 float angle = DX_PI * 2.0f * (float)i / segment;
		 VECTOR next = VAdd(m_Pos, VGet(cosf(angle) * detectRange, 0, sinf(angle) * detectRange)); 
		 DrawLine3D(prev, next, color); prev = next; 
	 } 
 }

// 索敵範囲内にプレイヤーがいるか判定 
 bool NormalEnemy::DetectPlayer(Player* player) 
 { 
	 // プレイヤーまでのベクトル 
	 VECTOR toPlayer = MyMath::VecCreate(m_Pos, player->GetPos()); float dist = MyMath::VecLong(toPlayer); 
	 // ★ 距離のみで判定（円形） 
	 return dist <= m_DetectRange; 
 }

 EnemyBase* NormalEnemy::Clone()
 {
	 NormalEnemy* clone = new NormalEnemy();

	 // ★ 基本パラメータだけコピー（位置・回転・スケールはコピーしない）
	 clone->SetMaxHP(GetMaxHP());
	 clone->SetHP(GetHP());
	 clone->m_DetectRange = m_DetectRange;

	 // モデル複製
	 clone->m_Handle = MV1DuplicateModel(m_Handle);

	 // マズルボーン再検索
	 const char* muzzleName[MUZZLE_NUM] = {
		 "MuzzleBorn_1","MuzzleBorn_2","MuzzleBorn_3","MuzzleBorn_4",
		 "MuzzleBorn_5","MuzzleBorn_6","MuzzleBorn_7","MuzzleBorn_8"
	 };
	 for (int i = 0; i < MUZZLE_NUM; ++i)
	 {
		 clone->m_MuzzleBone[i] = MV1SearchFrame(clone->m_Handle, muzzleName[i]);
	 }

	 // AABB 作成
	 clone->m_AABB = CollisionManager::GetInstance()->CreateAABB();
	 clone->m_AABB->SetTargetPos(&clone->m_Pos);
	 clone->m_AABB->SetLocalPos(VGet(0.0f, 1.0f, 0.0f));
	 clone->m_AABB->SetSize(VGet(2.0f, 4.0f, 2.0f));

	 // 内部状態初期化
	 clone->m_State = Patrol;
	 clone->m_CurrentMuzzle = 0;
	 clone->m_ShootTimer = 0.0f;
	 clone->m_PatrolTimer = 120;

	 return clone;
 }



void NormalEnemy::PatrolMove()
{
	if (--m_PatrolTimer <= 0)
	{
		float angle = (rand() % 360) * DX_PI / 180.0f;
		m_PatrolDir = VGet(cosf(angle), 0, sinf(angle));
		m_PatrolTimer = 120;
	}

	// 位置は変更せず、その場で向きを変える（索敵時に向きを変えて視覚的に回転させる）
	float targetRot = atan2f(m_PatrolDir.x, m_PatrolDir.z);
	m_Rot.y = MyMath::LerpAngle(m_Rot.y, targetRot, 0.05f);
}

void NormalEnemy::ChasePlayer(Player* player)
{
	VECTOR toPlayer = VSub(player->GetPos(), m_Pos);
	toPlayer.y = 0;

	if (MyMath::VecLong(toPlayer) < 0.001f) return;

	VECTOR dir = VNorm(toPlayer);
	// 位置は移動させず、その場でプレイヤー方向を向くだけにする
	float targetRot = atan2f(dir.x, dir.z);
	m_Rot.y = MyMath::LerpAngle(m_Rot.y, targetRot, 0.1f);
}

void NormalEnemy::Shoot()
{
	Player* player = PlayerManager::GetInstance()->GetControlledPlayer();
	if (!player) return;

	EnemyBullet* bullet =
		static_cast<EnemyBullet*>(
			BulletManager::GetInstance()->CreateBullet(ENEMY_BULLET));

	VECTOR pos;
	GetMuzzleWorldPos(pos);   // ← 位置だけ

	// プレイヤー方向へ撃つ
	VECTOR dir = VNorm(VSub(player->GetPos(), pos));

	bullet->SetPosition(pos);
	bullet->SetDirection(dir);
	bullet->SetTarget(player);
	bullet->Start();
}

// マズルのワールド座標を取得
void NormalEnemy::GetMuzzleWorldPos(VECTOR& outPos)
{
	int idx = m_MuzzleBone[m_CurrentMuzzle];

	// ★ これが正解
	MATRIX boneWorld = MV1GetFrameLocalWorldMatrix(m_Handle, idx);
	outPos = VGet(
		boneWorld.m[3][0],
		boneWorld.m[3][1],
		boneWorld.m[3][2]
	);

	m_CurrentMuzzle = (m_CurrentMuzzle + 1) % MUZZLE_NUM;
}





