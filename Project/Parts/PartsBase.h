#pragma once
#include <DxLib.h>
#include <string>
#include <vector>

// ★先に enum を宣言
enum PartsType
{
    PLAYER_LEFT_ARM,
    PLAYER_RIGHT_ARM,
    PLAYER_HEAD_AND_BODY,
    PLAYER_LEG,
    PARTS_TYPE_MAX,
    PARTS_TYPE_NONE = -1
};

class Player;
class CollisionCapsule;
class CollisionAABB;

class PartsBase
{
public:
    PartsBase();
    ~PartsBase();

public:
    virtual void Init();
    virtual void Load();
    virtual void Start();
    virtual void Step();
    virtual void Update();
    virtual void Draw();
    virtual void Fin();
    virtual PartsBase* Clone() = 0;

    void OnCollected();
    void SetVisible(bool visible);
    void SetActive(bool active) { m_IsActive = active; }
public:
    virtual void OnHitPlayer(Player* player) {};
    virtual CollisionCapsule* GetCapsule() = 0;
    virtual CollisionAABB* GetAABB() = 0;
    virtual std::string GetDisplayName() const = 0;

    // ★必須：パーツの種類を返す純粋仮想関数
    virtual PartsType GetPartsType() const = 0;

    int GetHandle() const { return m_Handle; }

     std::vector<std::string> GetUnionPartsName();

    const std::vector<int>& GetUnionHandles() const { return m_UnionHandles; }
    bool IsCollected() const { return m_IsCollected; }
    bool IsVisible() const { return m_IsVisible; }
protected:
    int m_Handle;
    VECTOR m_Pos;
    VECTOR m_Rot;
    VECTOR m_Scale;

    bool m_IsActive = true;
    bool m_Visible = true;
    bool m_IsCollected = false;
private:
    std::vector<int> m_UnionHandles;
    bool m_IsVisible = true;
};
