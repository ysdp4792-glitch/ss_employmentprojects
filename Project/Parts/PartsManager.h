#pragma once
#include <vector>
#include "PartsBase.h"

class PartsManager
{
public:
    static void CreateInstance() { if (!m_Instance) m_Instance = new PartsManager; }
    static PartsManager* GetInstance() { return m_Instance; }
    static void DeleteInstance() { if (m_Instance) { delete m_Instance; m_Instance = nullptr; } }

    void Init();
    void Load();
    void Start();
    void Step();
    void Update();
    void Draw();
    void Fin();

    PartsBase* CreateParts(PartsType type);
    void RemovePart(PartsBase* part);

    // パーツ収集状態
    void SetCollected(PartsType type, bool value);
    bool IsCollected(PartsType type) const { return m_Collected[type]; }

    // パーツリストを外部に参照渡し
    const std::vector<PartsBase*>& GetPartsList() const { return m_PartsList; }

    bool IsAllCollected() const;

private:
    PartsManager() = default;
    ~PartsManager() = default;

    static PartsManager* m_Instance;

    PartsBase* m_OriginalParts[PARTS_TYPE_MAX] = { nullptr };
    std::vector<PartsBase*> m_PartsList;

    bool m_Collected[PARTS_TYPE_MAX] = { false }; // 収集フラグ
};
