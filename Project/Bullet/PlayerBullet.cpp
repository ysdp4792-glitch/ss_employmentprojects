#include "PlayerBullet.h"
#include "../Player/PlayerManager.h"
#include "../Player/Player.h"
#include "../MyMath/MyMath.h"
#include "../Enemy/EnemyBase.h"
#include "../Collision/CollisionSphere.h"
#include "../Collision/CollisionManager.h"
#include <time.h>

#define SHOT_SPEED 0.5f

PlayerBullet::PlayerBullet()
{
	m_Handle = 0;
	m_Pos = VGet(0.0f, 0.0f, -5.0f);
	m_Rot = VGet(0.0f, 0.0f, 0.0f);
	m_Scale = VGet(0.0f, 0.0f, 0.0f);
	m_Lifetime = 5.0f; // 例: 5秒で消える
	m_Timer = 0.0f;
}

PlayerBullet::~PlayerBullet()
{
	Fin();
}

void PlayerBullet::Init()
{
}

void PlayerBullet::Load()
{
	m_Handle = MV1LoadModel("Data/Bullet/Bullet.x");
}

void PlayerBullet::Start()
{
	m_Sphere = CollisionManager::GetInstance()->CreateSphere();
	m_Sphere->SetTargetPos(&m_Pos);
	m_Sphere->SetLocalPos(VGet(0.0f, 0.0f, 0.0f));
	m_Sphere->SetRadius(0.3f);

	m_Rot = PlayerManager::GetInstance()->GetPlayer()->GetRot(); 

	float offsetDistance = 0.0f;

	if (m_OffsetType == OFFSET_LEFT) {
		offsetDistance = -1.7f;
	}
	else if (m_OffsetType == OFFSET_RIGHT) {
		offsetDistance = 1.7f;
	}

	// プレイヤーの座標
	VECTOR playerPos = PlayerManager::GetInstance()->GetPlayer()->GetPos();

	// ① プレイヤーを原点に移動させる平行移動行列
	VECTOR vecOrigin = MyMath::VecScale(playerPos, -1.0f);
	MATRIX playerOrigin = MyMath::MatTranslation(vecOrigin);

	// ② プレイヤーの手前に移動する平行移動行列を用意
	MATRIX distans = MyMath::MatTranslation(VGet(offsetDistance, 0.0f, 3.0f));

	// ④ カメラのヨー行列
	MATRIX yaw = MyMath::MatRotationYaw(m_Rot.y);

	// ⑤ プレイヤーを元の位置に戻す平行移動行列
	MATRIX matPlayerPos = MyMath::MatTranslation(playerPos);

	// ⑥ ①～⑤の行列を逆順に合成
	MATRIX mat = MyMath::MatMult(matPlayerPos, yaw);
	mat = MyMath::MatMult(mat, distans);
	mat = MyMath::MatMult(mat, playerOrigin);

	// ⑦ プレイヤーの位置から行列変換を計算したものがカメラの座標となる
	m_Pos = MyMath::MatTransform(mat, playerPos);
	
	m_Scale = VGet(1.0f, 1.0f, 1.0f);

}

void PlayerBullet::Step()
{
	m_Pos = MyMath::VecAdd(m_Pos, MyMath::VecScale(MyMath::VecForwardZX(m_Rot.y), SHOT_SPEED));

	// 時間計測
	m_Timer += 1.0f / 60.0f; // もしStepが1フレームごとなら、フレームレート60FPSとして計算
	if (m_Timer >= m_Lifetime)
	{
		m_IsDead = true;
	}
}

void PlayerBullet::Update()
{
	if (!m_IsDead)
	{
		// 3Dモデルの座標を設定する
		MV1SetPosition(m_Handle, m_Pos);
		// 3Dモデルの回転値を設定する
		MV1SetRotationXYZ(m_Handle, m_Rot);
		// 3Dモデルのスケールを設定する
		MV1SetScale(m_Handle, m_Scale);
	}
}

void PlayerBullet::Draw()
{
	if (!m_IsDead)
	{
		// 3Dモデルを描画する
		MV1DrawModel(m_Handle);
		m_Sphere->Draw();
	}
}

void PlayerBullet::Fin()
{
	// モデルをメモリから削除
	MV1DeleteModel(m_Handle);
}

BulletBase* PlayerBullet::Clone()
{

	// クローン用のオブジェクトを生成
	PlayerBullet* clone = new PlayerBullet;

	// 自身の中身をクローンにコピー
	*clone = *this;

	// 画像はDuplicateする必要がある
	clone->m_Handle = MV1DuplicateModel(m_Handle);

	// 出来上がったクローンを返却
	return clone;
}

void PlayerBullet::SetOffsetType( BulletOffsetType type)
{
	m_OffsetType = type;
}

void PlayerBullet::SetPosition(const VECTOR& pos)
{
	m_Pos = pos;
}

void PlayerBullet::HitEnemy(EnemyBase* enemy)
{
	if (!enemy) return;

	enemy->Damage(1);   // ダメージを与える
	m_IsDead = true;        // 弾は消える
}
