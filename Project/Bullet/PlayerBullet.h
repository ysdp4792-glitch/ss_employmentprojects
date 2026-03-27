#pragma once

#include "BulletBase.h"

enum PlayerBulletType
{
	BULLET_NORMAL,
	BULLET_FUSION_A,
	BULLET_FUSION_B,
};

enum BulletOffsetType {
	OFFSET_LEFT,
	OFFSET_RIGHT,
	OFFSET_CENTER
};

class Player;

class PlayerBullet : public BulletBase 
{
public:
	PlayerBullet();	// コンストラクタ
	~PlayerBullet();	// デストラクタ

public:
	void Init()override;	// 初期化 
	void Load()override;	// ロード 
	void Start()override;	// 開始 
	void Step()override;
	void Update()override;	// 更新
	void Draw()override;	// 描画
	void Fin()override;		// 終了
public:
	void SetOffsetType( BulletOffsetType type);
	void SetBulletType(PlayerBulletType type) { m_BulletType = type; }
	void SetPosition(const VECTOR& pos); 
	void HitEnemy(EnemyBase* enemy)override;
private:
	int m_Handle;
	VECTOR m_Pos;	// 座標
	VECTOR m_Rot;	// 回転
	VECTOR m_Scale;	// スケール
	BulletBase* Clone()override;
	BulletOffsetType m_OffsetType; 
	PlayerBulletType m_BulletType;
private:
	float m_Lifetime;    // 弾の寿命
	float m_Timer;       // 経過時間

};
