#include "PartsManager.h"
#include "PartsHeadBody.h"
#include "PartsRightArm.h"
#include "PartsLeftArm.h"
#include "PartsLeg.h"
#include <algorithm>

PartsManager* PartsManager::m_Instance = nullptr;

void PartsManager::Init()
{
    m_OriginalParts[PLAYER_LEFT_ARM] = new PartsLeftArm();
    m_OriginalParts[PLAYER_RIGHT_ARM] = new PartsRightArm();
    m_OriginalParts[PLAYER_HEAD_AND_BODY] = new PartsHeadBody();
    m_OriginalParts[PLAYER_LEG] = new PartsLeg();
}

void PartsManager::Load()
{
    for (int i = 0; i < PARTS_TYPE_MAX; i++)
    {
        if (m_OriginalParts[i])
            m_OriginalParts[i]->Load();
    }
}

void PartsManager::Start()
{
    for (auto parts : m_PartsList)
        parts->Start();
}

void PartsManager::Step()
{
    for (auto parts : m_PartsList)
        parts->Step();
}

void PartsManager::Update()
{
    for (auto parts : m_PartsList)
        parts->Update();
}

void PartsManager::Draw()
{
    for (auto parts : m_PartsList)
        parts->Draw();
}

void PartsManager::Fin()
{
    for (auto parts : m_PartsList)
        delete parts;
    m_PartsList.clear();

    for (int i = 0; i < PARTS_TYPE_MAX; i++)
    {
        delete m_OriginalParts[i];
        m_OriginalParts[i] = nullptr;
    }
}

PartsBase* PartsManager::CreateParts(PartsType type)
{
    if (!m_OriginalParts[type]) return nullptr;

    PartsBase* part = m_OriginalParts[type]->Clone();
    m_PartsList.push_back(part);
    return part;
}

void PartsManager::RemovePart(PartsBase* part)
{
    auto it = std::find(m_PartsList.begin(), m_PartsList.end(), part);
    if (it != m_PartsList.end()) {
        (*it)->SetVisible(false); // 非表示
        (*it)->OnCollected();     // 当たり判定削除
        // delete *it; ← 参照がなければOK、あるならNG
        m_PartsList.erase(it);    // リストから除外
    }

}


void PartsManager::SetCollected(PartsType type, bool value)
{
    m_Collected[type] = value;
}

bool PartsManager::IsAllCollected() const
{
    for (int i = 0; i < PARTS_TYPE_MAX; ++i) {
        if (!m_Collected[i]) return false;
    }
    return true;
}
