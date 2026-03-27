#pragma once
#include <Dxlib.h>
#include <string>
#include "../Parts/PartsBase.h"
#include "../Player/Player.h"

struct Bone
{
    int frameIndex = -1;
    MATRIX matrix;
    std::string name;
    int handle = -1;
};

// 胴体
struct BodyBones
{
    Bone spine;
    Bone spine1;
    Bone spine2;
    Bone spine3;
};

// 腕
struct ArmBones
{
    Bone shoulder;
    Bone upperArm;
    Bone forearm;
    Bone hand;
};

// 脚
struct LegBones
{
    Bone thigh;
    Bone knes;
    Bone shin;
    Bone toe;
    Bone heel;
};

// プレイヤー全体
struct PlayerSkeleton
{
    BodyBones body;

    ArmBones leftArm;
    ArmBones rightArm;

    LegBones leftLeg;
    LegBones rightLeg;

    Bone pelvisL;
    Bone pelvisR;
};

class PlayerBorn : public Player
{
public:
	PlayerBorn();
	~PlayerBorn();
public:
    void Load();
    void Update();
    void Draw();
    void Fin();
    const PlayerSkeleton& GetSkeleton() const { return skeleton; }
    MATRIX GetBoneMatrix(const Bone& bone);
    void AttachPart(PartsBase* part);
    MATRIX GetBoneMatrixByPart(PartsType type, size_t index);
    int GetPlayerHandle() const { return m_PlayerHandle;}
private:
    PlayerSkeleton skeleton;
    int m_PlayerHandle = -1;
};
