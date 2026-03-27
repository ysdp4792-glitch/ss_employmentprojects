#include "Player.h"
#include "PlayerManager.h"
#include "../Input/Input.h"
#include "../MyMath/MyMath.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionAABB.h"
#include "../Collision/CollisionSphere.h"
#include "../Collision/CollisionCapsule.h"
#include "../Stage/Stage.h"
#include "../Bullet/PlayerBullet.h"
#include "../Bullet/BulletManager.h"
#include "../Parts/PartsBase.h"
#include "../Parts/PartsManager.h"
#include "../Parts/PartsLeg.h"
#include "../Common/Common.h"
// Player 内ではボーン関連を使わないので Born ヘッダー参照はここで使わない
#include "../Born/PlayerBorn.h"
#include "../Enemy/EnemyManager.h"
#include "../Camera/CameraManager.h"
#include <algorithm>
#include <EffekseerForDXLib.h>

#define ROTATION_SPEED 0.1f
#define MOVE_SPEED 0.5f
#define ANIMATION_SPEED 0.5f

Player* Player::m_Instance = nullptr;

// 合体エフェクト用のエフェクトファイルパスを定義
static const char* effectUnion = "Data/Effect/UnionEffect.efkefc";

// 先頭付近に追加（std::clampが使えない場合の代替実装）
template <typename T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi)
{
    return (v < lo) ? lo : (hi < v) ? hi : v;
}

Player::Player() {
    m_Pos = VGet(0.0f, 0.0f, 0.0f);
    m_Rot = VGet(0.0f, 0.0f, 0.0f);
    m_Scale = VGet(0.0f, 0.0f, 0.0f);
    m_Move = VGet(0.0f, 0.0f, 0.0f);
    m_PrevPos = VGet(0.0f, 5.0f, -20.0f);
    m_Handle = -1;
    m_CurrentHandle = -1;
    m_Dir = VGet(0.0f, 0.0f, 0.0f);
    m_AABB = nullptr;
    m_SphereCollision = nullptr;
    m_Capsule = nullptr;
    m_Instance = this;
    m_Born = nullptr; // メンバは残すが Player.cpp 内で参照しない
    m_IsAssembly = false;
}

Player::~Player() {
    Fin();
}

void Player::Init()
{
    m_Handle = MV1LoadModel("Data/Player/Player.x");
    if (m_Handle < 0) {
        printfDx("Player_Base.x のロードに失敗しました\n");
    }

    m_CurrentHandle = m_Handle;

    // 位置・スケールなどは既存のまま
    m_Pos = VGet(0.0f, 5.0f, -20.0f);
    m_Rot = VGet(0.0f, 0.0f, 0.0f);
    m_Scale = VGet(0.05f, 0.05f, 0.05f);
    m_Visible = true;
    MV1SetPosition(m_CurrentHandle, m_Pos);
    MV1SetRotationXYZ(m_CurrentHandle, m_Rot);
    MV1SetScale(m_CurrentHandle, m_Scale);

    // 既存アニメ初期化（そのまま）
    if (MV1GetAnimNum(m_CurrentHandle) > 0) {
        int animIndex = 0;
        m_AnimationAttachIndex = MV1AttachAnim(m_CurrentHandle, animIndex);
        m_AnimationTotalTime = MV1GetAttachAnimTotalTime(m_CurrentHandle, m_AnimationAttachIndex);
        m_AnimationNowTime = 0.0f;
        m_IsLoopAnimation = true;
    }

    // ★ 合体エフェクトのロード（1回だけ）
    m_AssemblyEffectRes = LoadEffekseerEffect("Data/Effect/UnionEffect.efkefc");
    if (m_AssemblyEffectRes < 0) {
        printfDx("UnionEffect.efkefc のロードに失敗しました\n");
    }

    m_PlayingAssemblyEffect = -1;
    m_WaitingAssembly = false;
    m_PlayAssemblyEffect = false;
    m_AssemblyTimer = 0;
}

void Player::Load()
{
    // Player 内では Born のロード／生成を行わない（外部か合体時に行う）
}

void Player::Start() {
    m_Pos = VGet(0.0f, 3.0f, -25.0f);
    m_Rot = VGet(0.0f, 0.0f, 0.0f);
    m_Scale = VGet(0.4f, 0.4f, 0.4f);

    CollisionManager* collisionManager = CollisionManager::GetInstance();
    PlayerManager::GetInstance()->SetControlledPlayer(this);
    if (collisionManager) {
        m_AABB = collisionManager->CreateAABB();
        m_AABB->SetTargetPos(&m_Pos);
        m_AABB->SetLocalPos(VGet(0.0f, 0.5f, -1.0f));
        m_AABB->SetSize(VGet(1.0f, 1.0f, 1.0f));

        m_SphereCollision = collisionManager->CreateSphere();
        m_SphereCollision->SetTargetPos(&m_Pos);
        m_SphereCollision->SetLocalPos(VGet(0.0f, 0.5f, 0.0f));
        m_SphereCollision->SetRadius(0.5f);

        m_Capsule = collisionManager->CreateCapsule();
        m_Capsule->SetTargetPos(&m_Pos);
        m_Capsule->SetLocalPos(VGet(0.0f, 0.5f, 0.0f));
        m_Capsule->SetRadius(2.2f);
    }
}

void Player::Step()
{

    m_Move = VGet(0, 0, 0);
    const auto& analog = Input::GetAnalog();

    // ============================
    // ① ロックオン入力（方向キー右）
    // ============================
    int pov = Input::GetJoypadPOV(DX_INPUT_PAD1);

    static bool prevRight = false;
    bool nowRight = (pov == 9000); // ← 方向キー右だけ

    if (nowRight && !prevRight)
    {
        if (!m_IsLockOn)
        {
            m_LockOnTarget = FindNearestEnemy();
            m_IsLockOn = (m_LockOnTarget != nullptr);
        }
        else
        {
            m_IsLockOn = false;
            m_LockOnTarget = nullptr;
        }
    }

    prevRight = nowRight;



    // ============================
   // ② 向きの更新（TPS自然仕様）
   // ============================

   // ロックオン中は最優先で敵を向く
    if (m_IsLockOn && m_LockOnTarget)
    {
        VECTOR toEnemy = VSub(m_LockOnTarget->GetPos(), m_Pos);
        m_Rot.y = atan2f(toEnemy.x, toEnemy.z);
    }
    else
    {
        const auto& analog = Input::GetAnalog();

        // 前後入力がある → 前を向く（ここは今のままでもOK）
        if (fabsf(analog.moveY) > 0.1f)
        {
            m_Rot.y = 0.0f;
        }
        else
        {
            // ★ ロックオンしていないときはカメラ方向に向きを寄せる
            float camYaw = CameraManager::GetInstance()->GetCurrentYaw();

            // 0.15f は追従の速さ。好みで 0.1～0.3 の間で調整
            m_Rot.y = MyMath::LerpAngle(m_Rot.y, camYaw, 0.15f);
        }

        // 左右入力では向きを変えない（スライド移動）
    }


    // ============================
    // ③ 移動（常にシンプルに前後左右）
    // ============================
    VECTOR forward = VGet(0, 0, 1);  // 前方向（固定）
    VECTOR right = VGet(1, 0, 0);  // 右方向（固定）

    m_Move = VAdd(m_Move, VScale(forward, analog.moveY * MOVE_SPEED));
    m_Move = VAdd(m_Move, VScale(right, analog.moveX * MOVE_SPEED));

    // 斜め移動補正
    float len = VSize(m_Move);
    if (len > MOVE_SPEED)
        m_Move = VScale(VNorm(m_Move), MOVE_SPEED);

    // ============================
    // ④ 向きベクトル更新
    // ============================
    m_Dir = MyMath::VecForwardZX(m_Rot.y);
    m_Dir.y = 0;
    m_Dir = MyMath::VecNormalize(m_Dir);

    // ============================
    // ⑤ 上昇・下降
    // ============================
    if (Input::IsInputKey(KEY_SHIFT))
        m_VerticalSpeed += VERTICAL_ACCELERATION;
    else if (Input::IsInputKey(KEY_C))
        m_VerticalSpeed -= VERTICAL_ACCELERATION;
    else
        m_VerticalSpeed *= 0.9f;

    m_VerticalSpeed = clamp(m_VerticalSpeed, -VERTICAL_MAX_SPEED, VERTICAL_MAX_SPEED);
    m_Move.y = m_VerticalSpeed;

    m_IsWalking = fabsf(analog.moveY) > 0.1f;
}


void Player::Update()
{
    // ★ 合体エフェクト再生中の処理
    if (m_WaitingAssembly)
    {
        m_AssemblyTimer++;

        // エフェクトをプレイヤーに追従
        if (m_PlayingAssemblyEffect >= 0)
        {
            SetPosPlayingEffekseer3DEffect(
                m_PlayingAssemblyEffect,
                m_Pos.x,
                m_Pos.y + 2.0f,
                m_Pos.z
            );
        }

        // 一定時間経過で合体完了（ここでは 120 フレーム）
        if (m_AssemblyTimer > 120)
        {
            m_WaitingAssembly = false;
            m_PlayAssemblyEffect = false;

            // 合体状態に移行
            if (m_CompleteHandle >= 0)
            {
                m_CurrentHandle = m_CompleteHandle;
                m_IsAssembly = true;

                // 歩行アニメの設定（簡易版）
                m_WalkAnimAttachIndex = -1;
                m_WalkAnimTotalTime = 0.0f;
                m_WalkAnimNowTime = 0.0f;

                int animNum = MV1GetAnimNum(m_CompleteHandle);
                for (int ai = 0; ai < animNum; ++ai) {
                    int attach = MV1AttachAnim(m_CompleteHandle, ai);
                    float total = MV1GetAttachAnimTotalTime(m_CompleteHandle, attach);
                    if (attach >= 0 && total > 0.001f) {
                        m_WalkAnimAttachIndex = attach;
                        m_WalkAnimTotalTime = total;
                        break;
                    }
                }
            }
        }

        // 合体演出中も位置だけは更新したいならここで m_Pos 更新してもいいし、
        // 完全に止めたいなら何もしないで return する
        // ここでは「演出中は動かさない」前提で return
        return;
    }

    // ===== 通常の Update 処理 =====

    m_PrevPos = m_Pos;
    m_Pos = MyMath::VecAdd(m_Pos, m_Move);

    if (CanShoot() && Input::IsInputKey(KEY_SPACE)) {
        BulletManager::GetInstance()->FireBullet(m_Pos, PLAYER_BULLET, OFFSET_LEFT, this);
        BulletManager::GetInstance()->FireBullet(m_Pos, PLAYER_BULLET, OFFSET_RIGHT, this);
    }

    if (m_CurrentHandle >= 0) {
        MV1SetPosition(m_CurrentHandle, m_Pos);
        MV1SetRotationXYZ(m_CurrentHandle, m_Rot);
        MV1SetScale(m_CurrentHandle, m_Scale);
    }

    UpdateAnimation();
    UpdateCollectDialog();
    HitParts();
    UpdatePartsAttachment();
}

void Player::Draw()
{
    if (!m_Visible) return;

    // 合体演出中は合体前モデルを描画
    if (m_WaitingAssembly)
    {
        MV1SetPosition(m_Handle, m_Pos);
        MV1SetRotationXYZ(m_Handle, m_Rot);
        MV1SetScale(m_Handle, m_Scale);
        MV1DrawModel(m_Handle);

        for (auto* part : m_OwnedParts)
            if (part && part->IsVisible()) part->Draw();

        return;
    }

    // 通常時は m_CurrentHandle を描画
    if (m_CurrentHandle >= 0)
    {
        MV1SetPosition(m_CurrentHandle, m_Pos);
        MV1SetRotationXYZ(m_CurrentHandle, m_Rot);
        MV1SetScale(m_CurrentHandle, m_Scale);
        MV1DrawModel(m_CurrentHandle);
    }

    for (auto* part : m_OwnedParts)
        if (part && part->IsVisible()) part->Draw();
}


void Player::DrawHPBar()
{
    const int x = 20;
    const int y = 20;
    const int width = 200;
    const int height = 16;

    // 背景
    DrawBox(x - 2, y - 2, x + width + 2, y + height + 2, GetColor(0, 0, 0), TRUE);

    // HP割合
    float hpRate = (float)m_HP / (float)m_MaxHP;
    if (hpRate < 0.0f) hpRate = 0.0f;

    // HP本体
    DrawBox(x, y, x + (int)(width * hpRate), y + height, GetColor(0, 255, 0), TRUE);

    // 枠
    DrawBox(x, y, x + width, y + height, GetColor(255, 255, 255), FALSE);

    // 数値
    DrawFormatString(x, y + height + 4, GetColor(255, 255, 255), "HP %d / %d", m_HP, m_MaxHP);
}



void Player::Fin()
{
    for (auto* part : m_OwnedParts)
        delete part;
    m_OwnedParts.clear();

    if (m_CompleteHandle >= 0) {
        MV1DeleteModel(m_CompleteHandle);
        m_CompleteHandle = -1;
    }

    if (m_Handle >= 0) {
        MV1DeleteModel(m_Handle);
        m_Handle = -1;
    }
}


void Player::UpdateAnimation()
{
    if (m_CurrentHandle < 0) return;

    // 歩行アニメーションが有効なら再生時間をセットして更新
    if (m_WalkAnimAttachIndex >= 0)
    {
        if (m_IsWalking)
        {
            MV1SetAttachAnimTime(m_CurrentHandle, m_WalkAnimAttachIndex, m_WalkAnimNowTime);
            m_WalkAnimNowTime += ANIMATION_SPEED;
            if (m_WalkAnimNowTime > m_WalkAnimTotalTime)
            {
                // ループ
                m_WalkAnimNowTime = fmodf(m_WalkAnimNowTime, m_WalkAnimTotalTime);
            }
        }
        else
        {
            // 停止時は歩行アニメを最初に戻す（任意）
            m_WalkAnimNowTime = 0.0f;
            MV1SetAttachAnimTime(m_CurrentHandle, m_WalkAnimAttachIndex, 0.0f);
        }
    }

    // 既存の汎用アニメ処理は残す（もし他アニメがあるなら）
    if (m_AnimationAttachIndex >= 0)
    {
        MV1SetAttachAnimTime(m_CurrentHandle, m_AnimationAttachIndex, m_AnimationNowTime);
        m_AnimationNowTime += ANIMATION_SPEED;
        if (m_AnimationNowTime > m_AnimationTotalTime) {
            m_AnimationNowTime = m_IsLoopAnimation ? 0.0f : m_AnimationTotalTime;
        }
    }
}


void Player::HitStage(Stage* stage) {
    float playerRadius = m_Capsule->GetRadius();
    MV1_COLL_RESULT_POLY_DIM hitResult = stage->GetHitResult();
    for (int i = 0; i < hitResult.HitNum; i++) {
        MV1_COLL_RESULT_POLY polygon = hitResult.Dim[i];
        if (polygon.Normal.y <= 0.1f) continue;
        VECTOR lineStart = m_Capsule->GetWorldPos();
        VECTOR lineEnd = m_Pos;
        HITRESULT_LINE lineResult = HitCheck_Line_Triangle(lineStart, lineEnd,
            polygon.Position[0], polygon.Position[1], polygon.Position[2]);
        if (!lineResult.HitFlag) continue;

        // 高さだけ修正
        m_Pos.y = lineResult.Position.y;
        m_Move.y = 5.0f;
    }

    // 水平方向の衝突は別処理に分けたほうが良い
    if (m_CurrentHandle >= 0) MV1SetPosition(m_CurrentHandle, m_Pos);
}
void Player::HitParts() {
    PartsManager* partsManager = PartsManager::GetInstance();
    if (!partsManager) return;

    auto& partsList = partsManager->GetPartsList();

    // 収集対象を一時的にためる（走査中にコンテナを変更しない）
    std::vector<PartsBase*> toRemove;

    for (auto& part : partsList) {
        if (!part) continue;

        // プレイヤーと衝突した？
        if (part->GetAABB() && part->GetAABB()->CheckCapsule(m_Capsule)) {
            // フラグ類をセット
            partsManager->SetCollected(part->GetPartsType(), true);

            m_CollectMessage = part->GetDisplayName() + " を手に入れた！";
            m_CollectMessageTimer = 90;

            // 所有リストへ移す（重複しないようチェック）
            bool alreadyOwned = false;
            for (auto* owned : m_OwnedParts) {
                if (owned == part) { alreadyOwned = true; break; }
            }
            if (!alreadyOwned) {
                m_OwnedParts.push_back(part);
            }
            // マップ上からは消すが、所有パーツとしては描画する
            part->OnCollected();
            part->SetVisible(true); // ← ここで再度可視化

            // 実際の管理リストからは走査後に除外するため一時リストへ
            toRemove.push_back(part);
        }
    }

    // 走査が終わってからまとめて除外（RemovePart内で delete しない実装を前提）
    for (auto* p : toRemove) {
        partsManager->RemovePart(p);
    }

    // 全収集で合体
    if (partsManager->IsAllCollected()) {
        AssembleParts();
        ApplyControlState();
    }
}

void Player::UpdateCollectDialog() {
    if (m_CollectMessageTimer > 0)
    {
        m_CollectMessageTimer--;
    }
}

Player* Player::GetInstance() {
    return m_Instance;
}

void Player::DrawCollectDialog() {
    if (m_CollectMessageTimer > 0) {
        int x = 20;
        int y = SCREEN_HEIGHT - 110;
        DrawBox(x - 10, y - 10, x + 250, y + 30, GetColor(0, 0, 0), TRUE);
        DrawFormatString(x, y, GetColor(255, 255, 0), m_CollectMessage.c_str());
    }
}

void Player::AddPart(PartsBase* part)
{
    if (!part) return;
    m_GetParts.push_back(part);
    m_OwnedParts.push_back(part);
}

void Player::AssembleParts()
{
    if (m_IsAssembly) return;

    // すでにエフェクト待ちなら二重起動しない
    if (m_WaitingAssembly) return;

    // エフェクト再生開始
    if (m_AssemblyEffectRes >= 0)
    {
        m_PlayingAssemblyEffect = PlayEffekseer3DEffect(m_AssemblyEffectRes);

        if (m_PlayingAssemblyEffect >= 0)
        {
            // 位置：プレイヤーの少し上
            SetPosPlayingEffekseer3DEffect(
                m_PlayingAssemblyEffect,
                m_Pos.x,
                m_Pos.y + 2.0f,
                m_Pos.z
            );

            // スケール：まずは 1.0 で確認
            SetScalePlayingEffekseer3DEffect(
                m_PlayingAssemblyEffect,
                1.0f, 1.0f, 1.0f
            );

            m_WaitingAssembly = true;
            m_PlayAssemblyEffect = true;
            m_AssemblyTimer = 0;
        }
        else
        {
            printfDx("UnionEffect の再生に失敗しました\n");
        }
    }
    else
    {
        printfDx("UnionEffect がロードされていないため再生できません\n");
    }

    // パーツを非表示にする
    for (auto* part : m_OwnedParts) {
        if (part) part->SetVisible(false);
    }

    // 合体後モデルのロード（ここでは先にロードだけしておく）
    if (m_CompleteHandle < 0)
    {
        m_CompleteHandle = MV1LoadModel("Data/Player/Player_Union.x");
        if (m_CompleteHandle == -1) {
            printfDx("Player_Union.x の読み込みに失敗しました\n");
            m_CompleteHandle = -1;
        }
        else
        {
            MV1SetPosition(m_CompleteHandle, m_Pos);
            MV1SetScale(m_CompleteHandle, VGet(0.2f, 0.2f, 0.2f));
        }
    }

    // ここではまだ m_CurrentHandle は切り替えない
    // → エフェクト終了タイミングで切り替える
}



void Player::ApplyControlState()
{
    // プレイヤーは常に表示
    SetVisible(true);

    // Born は使わないので常に非表示
    if (m_Born) {
        m_Born->SetVisible(false);
    }

    // 操作対象は常に Player 自身
    PlayerManager::GetInstance()->SetControlledPlayer(this);
}

bool Player::CanShoot() const
{
    // 合体済み かつ エフェクト待ち中でない
    return m_IsAssembly && !m_WaitingAssembly;
}


void Player::UpdatePartsAttachment() {
    // Player 内ではボーンを参照しないため、簡易的にパーツをプレイヤー位置に合わせる処理にする
    for (auto* part : m_OwnedParts) {
        if (!part) continue;

        int handle = part->GetHandle();
        if (handle < 0) continue;

        part->SetVisible(true);

        // 簡易実装：パーツをプレイヤー位置に配置する（詳細なオフセットや回転はパーツ側で管理するか、別処理へ移譲）
        MATRIX mat = MGetTranslate(m_Pos);
        MV1SetMatrix(handle, mat);
    }
}

// ダメージ処理
void Player::Damage(int damage) {
    m_HP -= damage;
    if (m_HP < 0) m_HP = 0;
}

EnemyBase* Player::FindNearestEnemy()
{
    EnemyBase* nearest = nullptr;
    float nearestDist = 999999.0f;

    for (auto* enemy : EnemyManager::GetInstance()->GetEnemyList())
    {
        if (!enemy) continue;

        float dist = VSize(VSub(enemy->GetPos(), m_Pos));
        if (dist < nearestDist)
        {
            nearestDist = dist;
            nearest = enemy;
        }
    }
    return nearest;
}


