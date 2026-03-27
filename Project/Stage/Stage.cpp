#include <Dxlib.h>
#include "Stage.h"

Stage::Stage()
{
	m_Handle = 0;
	m_CollisionHandle = 0;
	m_CollisionResult = {};
}

Stage::~Stage()
{
	Fin();
}

void Stage::Load()
{
	// 描画するモデルをロード
	m_Handle = MV1LoadModel("Data/Map/MapModel.x");
	// Zバッファを有効にする
	MV1SetUseZBuffer(m_Handle, TRUE);
	// 当たり判定用のモデルをロード
	m_CollisionHandle = MV1LoadModel("Data/Map/MapModelSample.x");
}

void Stage::Start()
{
	// 衝突情報を構築
	MV1SetupCollInfo(m_CollisionHandle);
}

void Stage::Draw()
{
	MV1DrawModel(m_Handle);
}

void Stage::Fin()
{
	// 構築した衝突情報を削除
	MV1TerminateCollInfo(m_CollisionHandle);
	// 画像を削除
	MV1DeleteModel(m_Handle);
	// 当たり判定用のモデルも削除
	MV1DeleteModel(m_CollisionHandle);

}

bool Stage::CheckCollisionCapsule(VECTOR pos1, VECTOR pos2, float r)
{
	// ステージとカプセルの当たり判定
	m_CollisionResult = MV1CollCheck_Capsule(m_CollisionHandle, -1, pos1,pos2,r);
	// HitNumが0なら当たっていない
	return m_CollisionResult.HitNum != 0;
}

bool Stage::CheckCollisionSphere(VECTOR center, float radius)
{
	// 例：当たり判定用のステージモデルとのチェック（DxLibの当たり判定機能を仮定）
	// ステージの当たり判定モデルハンドル：m_HitModel
	m_CollisionResult = MV1CollCheck_Sphere(m_CollisionHandle, -1, center, radius); 
	return m_CollisionResult.HitNum != 0;
}
