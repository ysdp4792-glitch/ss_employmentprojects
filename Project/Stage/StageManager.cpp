#include "StageManager.h"
#include "Stage.h"

StageManager* StageManager::m_Instance = nullptr;

StageManager::StageManager()
{
	m_Stage = nullptr;
}

StageManager::~StageManager()
{
	Fin();
}

void StageManager::Init()
{
	m_Stage = new Stage;
}

void StageManager::Load()
{
	m_Stage->Load();
}

void StageManager::Draw()
{
	m_Stage->Draw();
}

void StageManager::Fin()
{
	delete m_Stage;
}


