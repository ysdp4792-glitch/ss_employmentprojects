#include <DxLib.h>
#include "PlayerBorn.h"
#include "../MyMath/MyMath.h"
#include "../Parts/PartsBase.h"
#include "../Player/Player.h"
#include "../Player/PlayerManager.h"

PlayerBorn::PlayerBorn()
{
}

PlayerBorn::~PlayerBorn()
{
}

void PlayerBorn::Load() {
    // ★ すでにロード済みならスキップ
    if (m_PlayerHandle != -1) return;

    m_PlayerHandle = MV1LoadModel("Data/Player/Player_Born.x");
    if (m_PlayerHandle == -1) return;

    Player* player = PlayerManager::GetInstance()->GetPlayer();
    for (auto* part : player->GetParts()) {
        if (!part) continue;          // null チェック
        part->Load();                 // パーツ自体をロード
        AttachPart(part);            // ハンドルを骨に対応付け
    }
}

void PlayerBorn::Update() 
{
    Player* player = PlayerManager::GetInstance()->GetPlayer();
    MV1SetPosition(m_PlayerHandle, player->GetPos());

   

    skeleton.leftArm.hand.matrix = GetBoneMatrix(skeleton.leftArm.hand);
    skeleton.rightArm.hand.matrix = GetBoneMatrix(skeleton.rightArm.hand);
}

void PlayerBorn::Draw()
{
	//SetUseLighting(false);
    //DxLib::SetLightEnable(false);
    if (!m_Visible) return;
    auto DrawPartWithTransform = [&](int handle, const MATRIX& boneMatrix,
        const VECTOR& scale, const VECTOR& rot, const VECTOR& trans)
        {
            if (handle < 0) return;

            MATRIX mScale = MGetScale(scale);
            MATRIX mRotX = MGetRotX(rot.x);
            MATRIX mRotY = MGetRotY(rot.y);
            MATRIX mRotZ = MGetRotZ(rot.z);

            MATRIX local = mScale;
            local = MMult(local, mRotX);
            local = MMult(local, mRotY);
            local = MMult(local, mRotZ);

            // ★ プレイヤー位置は入れない！
            // MATRIX mTrans = MGetTranslate(trans);

            MATRIX final = MMult(boneMatrix, local);

            MV1SetMatrix(handle, final);
            MV1DrawModel(handle);
        };

    // ===================
    // 各パーツ描画処理
    // ===================

    Player* player = PlayerManager::GetInstance()->GetPlayer();

    VECTOR scaleBody = VGet(0.05f, 0.05f, 0.05f);
    VECTOR scaleLeftArm = VGet(0.05f, 0.05f, 0.05f);
    VECTOR scaleRightArm = VGet(0.05f, 0.05f, 0.05f);
    VECTOR scaleLeftLeg = VGet(0.05f, 0.05f, 0.05f);
    VECTOR scaleRightLeg = VGet(0.05f, 0.05f, 0.05f);
    VECTOR scalePelvis = VGet(0.05f, 0.05f, 0.05f);

    VECTOR rotZero = player->GetRot();
    VECTOR transZero = player->GetPos();

    // 胴体
    DrawPartWithTransform(skeleton.body.spine.handle, GetBoneMatrix(skeleton.body.spine), scaleBody, rotZero, transZero);
    DrawPartWithTransform(skeleton.body.spine1.handle, GetBoneMatrix(skeleton.body.spine1), scaleBody, rotZero, transZero);
    DrawPartWithTransform(skeleton.body.spine2.handle, GetBoneMatrix(skeleton.body.spine2), scaleBody, rotZero, transZero);
    DrawPartWithTransform(skeleton.body.spine3.handle, GetBoneMatrix(skeleton.body.spine3), scaleBody, rotZero, transZero);

    // 左腕
    DrawPartWithTransform(skeleton.leftArm.shoulder.handle, GetBoneMatrix(skeleton.leftArm.shoulder), scaleLeftArm, rotZero, transZero);
    DrawPartWithTransform(skeleton.leftArm.upperArm.handle, GetBoneMatrix(skeleton.leftArm.upperArm), scaleLeftArm, rotZero, transZero);
    DrawPartWithTransform(skeleton.leftArm.forearm.handle, GetBoneMatrix(skeleton.leftArm.forearm), scaleLeftArm, rotZero, transZero);
    DrawPartWithTransform(skeleton.leftArm.hand.handle, GetBoneMatrix(skeleton.leftArm.hand), scaleLeftArm, rotZero, transZero);

    // 右腕
    DrawPartWithTransform(skeleton.rightArm.shoulder.handle, GetBoneMatrix(skeleton.rightArm.shoulder), scaleRightArm, rotZero, transZero);
    DrawPartWithTransform(skeleton.rightArm.upperArm.handle, GetBoneMatrix(skeleton.rightArm.upperArm), scaleRightArm, rotZero, transZero);
    DrawPartWithTransform(skeleton.rightArm.forearm.handle, GetBoneMatrix(skeleton.rightArm.forearm), scaleRightArm, rotZero, transZero);
    DrawPartWithTransform(skeleton.rightArm.hand.handle, GetBoneMatrix(skeleton.rightArm.hand), scaleRightArm, rotZero, transZero);

    //左脚
    DrawPartWithTransform(skeleton.leftLeg.thigh.handle, GetBoneMatrix(skeleton.leftLeg.thigh), scaleLeftLeg, rotZero, transZero);
    DrawPartWithTransform(skeleton.leftLeg.knes.handle, GetBoneMatrix(skeleton.leftLeg.knes), scaleLeftLeg, rotZero, transZero);
    DrawPartWithTransform(skeleton.leftLeg.shin.handle, GetBoneMatrix(skeleton.leftLeg.shin), scaleLeftLeg, rotZero, transZero);
    DrawPartWithTransform(skeleton.leftLeg.toe.handle, GetBoneMatrix(skeleton.leftLeg.toe), scaleLeftLeg, rotZero, transZero);
    DrawPartWithTransform(skeleton.leftLeg.heel.handle, GetBoneMatrix(skeleton.leftLeg.heel), scaleLeftLeg, rotZero, transZero);
    
    //右脚
    DrawPartWithTransform(skeleton.rightLeg.thigh.handle, GetBoneMatrix(skeleton.rightLeg.thigh), scaleRightLeg, rotZero, transZero);
    DrawPartWithTransform(skeleton.rightLeg.knes.handle, GetBoneMatrix(skeleton.rightLeg.knes), scaleRightLeg, rotZero, transZero);
    DrawPartWithTransform(skeleton.rightLeg.shin.handle, GetBoneMatrix(skeleton.rightLeg.shin), scaleRightLeg, rotZero, transZero);
    DrawPartWithTransform(skeleton.rightLeg.toe.handle, GetBoneMatrix(skeleton.rightLeg.toe), scaleRightLeg, rotZero, transZero);
    DrawPartWithTransform(skeleton.rightLeg.heel.handle, GetBoneMatrix(skeleton.rightLeg.heel), scaleRightLeg, rotZero, transZero);
    
    // 骨盤
    DrawPartWithTransform(skeleton.pelvisL.handle, GetBoneMatrix(skeleton.pelvisL), scalePelvis, rotZero, transZero); 
    DrawPartWithTransform(skeleton.pelvisR.handle, GetBoneMatrix(skeleton.pelvisR), scalePelvis, rotZero, transZero);

}

void PlayerBorn::Fin()
{
}

MATRIX PlayerBorn::GetBoneMatrix(const Bone& bone)
{
    if (bone.frameIndex == -1) return MGetIdent();//単位行列
    return MV1GetFrameLocalWorldMatrix(m_PlayerHandle, bone.frameIndex);// ローカル座標をワールド座標に変換
}
// 各ボーンの行列を取得
void PlayerBorn::AttachPart(PartsBase* part)
{
    auto& handles = part->GetUnionHandles(); // ← すでにロード済みのハンドル
    for (size_t i = 0; i < handles.size(); ++i) {
        int handle = handles[i];
        if (handle < 0) continue;
        switch (part->GetPartsType()) 
        {
        case PLAYER_HEAD_AND_BODY:
            if (i == 0) { skeleton.body.spine.frameIndex = MV1SearchFrame(m_PlayerHandle, "spine"); skeleton.body.spine.handle = handle; } 
            if (i == 1) { skeleton.body.spine1.frameIndex = MV1SearchFrame(m_PlayerHandle, "spine.001"); skeleton.body.spine1.handle = handle; } 
            if (i == 2) { skeleton.body.spine2.frameIndex = MV1SearchFrame(m_PlayerHandle, "spine.002"); skeleton.body.spine2.handle = handle; } 
            if (i == 3) { skeleton.body.spine3.frameIndex = MV1SearchFrame(m_PlayerHandle, "spine.003"); skeleton.body.spine3.handle = handle; } 
            break;

        case PLAYER_LEFT_ARM:
            if (i == 0) { skeleton.leftArm.shoulder.frameIndex = MV1SearchFrame(m_PlayerHandle, "shoulder.L"); skeleton.leftArm.shoulder.handle = handle; } 
            if (i == 1) { skeleton.leftArm.upperArm.frameIndex = MV1SearchFrame(m_PlayerHandle, "upper_arm.L"); skeleton.leftArm.upperArm.handle = handle; } 
            if (i == 2) { skeleton.leftArm.forearm.frameIndex = MV1SearchFrame(m_PlayerHandle, "forearm.L"); skeleton.leftArm.forearm.handle = handle; } 
            if (i == 3) { skeleton.leftArm.hand.frameIndex = MV1SearchFrame(m_PlayerHandle, "hand.L"); skeleton.leftArm.hand.handle = handle; } 
            break; 

        case PLAYER_RIGHT_ARM: 
            if (i == 0) { skeleton.rightArm.shoulder.frameIndex = MV1SearchFrame(m_PlayerHandle, "shoulder.R"); skeleton.rightArm.shoulder.handle = handle; } 
            if (i == 1) { skeleton.rightArm.upperArm.frameIndex = MV1SearchFrame(m_PlayerHandle, "upper_arm.R"); skeleton.rightArm.upperArm.handle = handle; } 
            if (i == 2) { skeleton.rightArm.forearm.frameIndex = MV1SearchFrame(m_PlayerHandle, "forearm.R"); skeleton.rightArm.forearm.handle = handle; } 
            if (i == 3) { skeleton.rightArm.hand.frameIndex = MV1SearchFrame(m_PlayerHandle, "hand.R"); skeleton.rightArm.hand.handle = handle; } 
            break;

        case PLAYER_LEG:
            // 右足
            if (i == 0) { skeleton.pelvisR.frameIndex = MV1SearchFrame(m_PlayerHandle, "pelvis.R"); skeleton.pelvisR.handle = handle; }
            if (i == 1) { skeleton.rightLeg.thigh.frameIndex = MV1SearchFrame(m_PlayerHandle, "thigh.R"); skeleton.rightLeg.thigh.handle = handle; }
            if (i == 2) { skeleton.rightLeg.knes.frameIndex = MV1SearchFrame(m_PlayerHandle, "knee.R"); skeleton.rightLeg.knes.handle = handle; }
            if (i == 3) { skeleton.rightLeg.shin.frameIndex = MV1SearchFrame(m_PlayerHandle, "shin.R"); skeleton.rightLeg.shin.handle = handle; }
            if (i == 4) { skeleton.rightLeg.toe.frameIndex = MV1SearchFrame(m_PlayerHandle, "toe.R"); skeleton.rightLeg.toe.handle = handle; }
            if (i == 5) { skeleton.rightLeg.heel.frameIndex = MV1SearchFrame(m_PlayerHandle, "heel.02.R"); skeleton.rightLeg.heel.handle = handle; }
          
            // 左足
            if (i == 0) { skeleton.pelvisL.frameIndex = MV1SearchFrame(m_PlayerHandle, "pelvis.L"); skeleton.pelvisL.handle = handle; }
            if (i == 1) { skeleton.leftLeg.thigh.frameIndex = MV1SearchFrame(m_PlayerHandle, "thigh.L"); skeleton.leftLeg.thigh.handle = handle; }
            if (i == 2) { skeleton.leftLeg.knes.frameIndex = MV1SearchFrame(m_PlayerHandle, "knee.L"); skeleton.leftLeg.knes.handle = handle; }
            if (i == 3) { skeleton.leftLeg.shin.frameIndex = MV1SearchFrame(m_PlayerHandle, "shin.L"); skeleton.leftLeg.shin.handle = handle; }
            if (i == 4) { skeleton.leftLeg.toe.frameIndex = MV1SearchFrame(m_PlayerHandle, "toe.L"); skeleton.leftLeg.toe.handle = handle; }
            if (i == 5) { skeleton.leftLeg.heel.frameIndex = MV1SearchFrame(m_PlayerHandle, "heel.02.L"); skeleton.leftLeg.heel.handle = handle; }

            break;

        default:
            break;
        }
    }
}

