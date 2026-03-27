#include "Camera.h"
#include "../Player/PlayerManager.h"
#include "../Player/Player.h"
#include "../Input/Input.h"
#include "../MyMath/MyMath.h"
#include "../Enemy/EnemyBase.h"

#define CAMERA_NEAR_CLIP 0.01f
#define CAMERA_FAR_CLIP 1000.0f

#define PLAYER_DISTANCE -10.0f
#define ROTATION_SPEED 0.025f

#define X_ROTATION_MAX (DX_PI_F * 0.49f)

// 基底クラスのコンストラクタ呼ぶ際は追加で書く
Camera::Camera() : CameraBase()
{
    m_TargetPlayer = nullptr;
    m_IsAssembled = false;
}

Camera::~Camera()
{
    Fin();
}

void Camera::Init()
{
}

void Camera::Load()
{
}

void Camera::Start()
{
    SetCameraNearFar(CAMERA_NEAR_CLIP, CAMERA_FAR_CLIP);

    m_UpVec = VGet(0.0f, 1.0f, 0.0f);

    m_TargetPlayer = PlayerManager::GetInstance()->GetPlayer();

    // ★ 初期カメラ角度を正面に向ける
    m_Rot = VGet(0.0f, 0.0f, 0.0f);
}

void Camera::Step()
{
    Player* player = PlayerManager::GetInstance()->GetPlayer();
    if (!player) return;

    // ★ ロックオン中はカメラ回転を無効化
    if (player->IsLockOn())
    {
        return;
    }

    AnalogState a = Input::GetAnalog();

    // 右スティックでカメラ回転
    m_Rot.y += a.yaw * 0.05f;    // 左右
    m_Rot.x += a.pitch * 0.05f;  // 上下

    // キー入力処理
    if (Input::IsInputKey(KEY_A)) m_Rot.y -= ROTATION_SPEED;
    if (Input::IsInputKey(KEY_D)) m_Rot.y += ROTATION_SPEED;
    if (Input::IsInputKey(KEY_W)) m_Rot.x += ROTATION_SPEED;
    if (Input::IsInputKey(KEY_S)) m_Rot.x -= ROTATION_SPEED;

    // X軸回転の制限
    if (m_Rot.x >= X_ROTATION_MAX) m_Rot.x = X_ROTATION_MAX;
    else if (m_Rot.x <= -X_ROTATION_MAX) m_Rot.x = -X_ROTATION_MAX;
}


void Camera::Update()
{
    Player* player = PlayerManager::GetInstance()->GetPlayer();
    if (!player) return;

    VECTOR playerPos = player->GetPos();

    // ===== プレイヤーの背後方向（固定） =====
    // ※ カメラの向きではなく、プレイヤーの向きを使う
    VECTOR backDir = player->GetDir();
    backDir.y = 0;
    backDir = VNorm(backDir);

    float backOffset = 25.0f;
    float upOffset = 15.0f;

    // プレイヤーの後ろ固定
    VECTOR baseCamPos = VAdd(playerPos,
        VAdd(VScale(backDir, -backOffset), VGet(0, upOffset, 0)));

    // ===== ロックオン中 =====
    if (player->IsLockOn())
    {
        VECTOR enemyPos = player->GetLockOnTarget()->GetPos();

        // プレイヤーと敵の中間点
        VECTOR mid = VScale(VAdd(playerPos, enemyPos), 0.5f);

        // スムージング
        float t = 0.15f;
        m_Pos = VAdd(VScale(m_Pos, 1.0f - t), VScale(baseCamPos, t));
        m_Target = VAdd(VScale(m_Target, 1.0f - t), VScale(mid, t));

        SetCameraPositionAndTargetAndUpVec(m_Pos, m_Target, m_UpVec);
        return;
    }

    // ===== 通常カメラ =====
    m_Pos = baseCamPos;
    m_Target = VAdd(playerPos, VGet(0, 2, 0));

    SetCameraPositionAndTargetAndUpVec(m_Pos, m_Target, m_UpVec);
}


void Camera::Draw()
{
    /*
    DrawFormatString(0, 60, GetColor(255, 255, 255),
        "カメラの座標[%f, %f, %f]", m_Pos.x, m_Pos.y, m_Pos.z);
    */
}

void Camera::Fin()
{
}
