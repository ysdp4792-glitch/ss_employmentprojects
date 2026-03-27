#pragma once
#include <DxLib.h>
#include <vector>
#include <string>

class CollisionAABB;
class CollisionSphere;
class CollisionCapsule;
class Stage;
class PartsBase;
class PlayerBorn;
class EnemyBase;

enum PlayerAnimationType
{
    PLAYER_ANIMATION_CANNON,
    PLAYER_ANIMATION_WAIT,
};

enum PlayerState
{
    STATE_NORMAL,
    STATE_COMBINE
};

class Player
{
public:
    Player();
    ~Player();

    // ライフサイクル
    void Init();
    void Load();
    void Start();
    void Step();
    void Update();
    void Draw();
    void DrawHPBar();
    void Fin();

    // アニメーション
    void PlayAnimation(PlayerAnimationType anim, bool isLoop);
    void UpdateAnimation();

    // 当たり判定関連
    CollisionAABB* GetAABB() { return m_AABB; }
    CollisionCapsule* GetCapsuleCollision() { return m_Capsule; }
    CollisionSphere* GetSphereCollision() { return m_SphereCollision; }

    // 衝突処理
    void HitStage(Stage* stage);
    void HitParts();

    // パーツ管理
    void AddPart(PartsBase* part);
    void AssembleParts();                // ★合体処理 (エフェクト再生を挟む)
    void UpdatePartsAttachment();
    bool IsAssembled() const { return m_IsAssembly; }

    // 収集UI
    void DrawCollectDialog();
    void UpdateCollectDialog();

    // インスタンス
    static Player* GetInstance();

    // モデル・描画関連
    int GetCurrentHandle() const { return m_CurrentHandle; }
    void SetCurrentHandle(int handle) { m_CurrentHandle = handle; }
    void SetVisible(bool visible) { m_Visible = visible; }

    // 位置・向き関連
    void SetPos(const VECTOR& pos) { m_Pos = pos; }
    VECTOR GetPos() const { return m_Pos; }
    VECTOR GetPrevPos() const { return m_PrevPos; }
    VECTOR GetDir() const { return m_Dir; }
    const VECTOR& GetRot() const { return m_Rot; }
    // ボーン情報
    PlayerBorn* GetBorn() { return m_Born; }
    std::vector<PartsBase*> GetParts() { return m_GetParts; }
    void ApplyControlState();
    void SetBorn(PlayerBorn* born) { m_Born = born; }

	bool CanShoot() const;
    bool IsPlayingAssemblyEffect() const { return m_PlayAssemblyEffect; }
    // ダメージを受ける
    void Damage(int damage);
    bool IsDead() const { return m_HP <= 0; }
    int  GetHP() const { return m_HP; }
    int GetMaxHP() const { return m_MaxHP; }

    EnemyBase* FindNearestEnemy();
    bool IsLockOn() const { return m_IsLockOn; }
    EnemyBase* GetLockOnTarget() const { return m_LockOnTarget; }
private:
    // モデル・アニメーション関連
    int m_Handle = -1;
    int m_AnimationAttachIndex = -1;
    int m_NowAnimation = -1;
    float m_AnimationTotalTime = 0.0f;
    float m_AnimationNowTime = 0.0f;
    bool m_IsLoopAnimation = false;

    // 位置・移動
    VECTOR m_Pos = VGet(0, 0, 0);
    VECTOR m_Rot = VGet(0, 0, 0);
    VECTOR m_Scale = VGet(1, 1, 1);
    VECTOR m_Move = VGet(0, 0, 0);
    VECTOR m_PrevPos = VGet(0, 0, 0);
    VECTOR m_Dir = VGet(0, 0, 1);   // 初期正面方向

    // 上下移動
    float m_VerticalSpeed = 0.0f;
    const float VERTICAL_ACCELERATION = 0.02f;
    const float VERTICAL_MAX_SPEED = 0.3f;
    const float VERTICAL_DECELERATION = 0.01f;

    // 当たり判定
    CollisionAABB* m_AABB = nullptr;
    CollisionSphere* m_SphereCollision = nullptr;
    CollisionCapsule* m_Capsule = nullptr;

    // パーツ
    std::vector<PartsBase*> m_OwnedParts;
    bool m_IsAssembly = false;
    // UI関連
    int m_CollectMessageTimer = 0;
    std::string m_CollectMessage;

    // モデル管理
    int m_BaseHandle = -1;
    int m_CurrentHandle = -1;
    int m_CompleteHandle = -1; // 合体後モデルハンドル
    // ボーン
    PlayerBorn* m_Born = nullptr;

    // Effekseer (合体エフェクト) 関連
    int m_AssemblyEffectRes = -1;        // efk リソースハンドル（LoadEffekseerEffect の戻り値）
    int m_PlayingAssemblyEffect = -1;    // 再生ハンドル（PlayEffekseer3DEffect の戻り値）
    bool m_WaitingAssembly = false;      // エフェクト再生中で合体待ち

    // シングルトン
    static Player* m_Instance;

    std::vector<PartsBase*>m_GetParts;
	// HP関連
    int m_MaxHP = 100;
    int m_HP = 100;
    // 歩行アニメーション用
    int m_WalkAnimAttachIndex = -1; // MV1AttachAnim の戻り値（歩行アニメ用）
    float m_WalkAnimTotalTime = 0.0f;
    float m_WalkAnimNowTime = 0.0f;
    bool m_IsWalking = false;

    PlayerState playerState = STATE_NORMAL;

    bool m_PlayAssemblyEffect = false;
    int m_AssemblyTimer = 0;
    int m_EffectHandle = -1;

    EnemyBase* m_LockOnTarget = nullptr;
    bool m_IsLockOn = false;
protected:
    bool m_Visible = false;
};
