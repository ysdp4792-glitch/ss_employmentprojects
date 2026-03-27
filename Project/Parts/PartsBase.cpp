#include "PartsBase.h"
#include "../Collision/CollisionManager.h"
#include "../Common/Common.h"

PartsBase::PartsBase()
{
	m_Handle = 0;
	m_Pos = VGet(0.0f,0.0f,0.0f);
	m_Rot = VGet(0.0f, 0.0f, 0.0f);
	m_Scale = VGet(0.0f, 0.0f, 0.0f);

}

PartsBase::~PartsBase()
{
	Fin();
}

void PartsBase::Init()
{
}

void PartsBase::Load()
{
    auto files = GetUnionPartsName();
    for (auto& f : files) {
        int handle = MV1LoadModel(f.c_str());

        if (handle != -1) {
            m_UnionHandles.push_back(handle);
            //printfDx("ロード成功: %s\n", f.c_str()); // デバッグ出力
        }
        else {
            //printfDx("ロード失敗: %s\n", f.c_str());
        }
    }
}


void PartsBase::Start()
{
	
}

void PartsBase::Step()
{
}

void PartsBase::Update()
{
	if (!m_IsActive) return;
}

void PartsBase::Draw()
{
    // 落ちている状態なら m_Visible を尊重
    if (!m_IsCollected) {
        if (!m_Visible) return;
    }

    // 所有状態なら常に描画（プレイヤー側で制御）
    for (int h : m_UnionHandles) {
        if (h < 0) continue;
        MATRIX mTrans = MGetTranslate(m_Pos);
        MV1SetMatrix(h, mTrans);
        MV1DrawModel(h);
    }
}

void PartsBase::Fin()
{
}

void PartsBase::SetVisible(bool visible) 
{
    m_Visible = visible;
    for (int handle : m_UnionHandles) {
        if (handle >= 0) {
            MV1SetVisible(handle, visible ? TRUE : FALSE);
        }
    }
}

 std::vector<std::string> PartsBase::GetUnionPartsName()
{
    switch (GetPartsType())
    {
    case PLAYER_HEAD_AND_BODY:
        return {
            "Data/Parts/HeadBody/Player_Leg_V.x",
            "Data/Parts/HeadBody/Player_Body.x",
            "Data/Parts/HeadBody/Player_Neck.x",
            "Data/Parts/HeadBody/Player_Face.x"
        };

    case PLAYER_LEFT_ARM:
        return {
            "Data/Parts/LeftArm/Left_Shoulder.x",
            "Data/Parts/LeftArm/Left_Upper_Arm.x",
            "Data/Parts/LeftArm/Left_Arm_Elbow.x",
            "Data/Parts/LeftArm/Left_Arm_Forearm.x"
        };

    case PLAYER_RIGHT_ARM:
        return {
            "Data/Parts/RightArm/Right_Shoulder.x",
            "Data/Parts/RightArm/Right_Upper_Arm.x",
            "Data/Parts/RightArm/Right_Arm_Elbow.x",
            "Data/Parts/RightArm/Right_Arm_Forearm.x"
        };

    case PLAYER_LEG:
        return {
            // 右足
            "Data/Parts/Leg/RightLeg/Player_Right_Coxa.x",
            "Data/Parts/Leg/RightLeg/Player_Right_Thigh.x",
            "Data/Parts/Leg/RightLeg/Player_Right_Knees.x",
            "Data/Parts/Leg/RightLeg/Player_Right_Shin.x",
            "Data/Parts/Leg/RightLeg/Player_Right_Toes.x",
            "Data/Parts/Leg/RightLeg/Player_Right_Heels.x",
           

            // 左足
            "Data/Parts/Leg/LeftLeg/Player_Left_Coxa.x",
            "Data/Parts/Leg/LeftLeg/Player_Left_Thigh.x",
            "Data/Parts/Leg/LeftLeg/Player_Left_Knees.x",
            "Data/Parts/Leg/LeftLeg/Player_Left_Shin.x",
            "Data/Parts/Leg/LeftLeg/Player_Left_Toes.x",
            "Data/Parts/Leg/LeftLeg/Player_Left_Heels.x"
        };

    default:
        return {};
    }
}
// メッシュを非表示にするなどの回収処理
void PartsBase::OnCollected()
{
    m_IsCollected = true;
	// --- カプセル削除 ---
	if (GetCapsule()) {
		CollisionManager::GetInstance()->DeleteCapsule(GetCapsule());
	}

	// --- AABB削除 ---
	if (GetAABB()) {
		CollisionManager::GetInstance()->DeleteAABB(GetAABB());
	}
}
