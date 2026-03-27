#include "PlayerRobot.h"
#include "../Born/PlayerBorn.h"
#include "../Parts/PartsHeadBody.h"
#include "../Parts/PartsLeftArm.h"
#include "../Parts/PartsRightArm.h"
#include "../Parts/PartsLeg.h"

PlayerRobot::PlayerRobot()
{
}

PlayerRobot::~PlayerRobot()
{
}

void PlayerRobot::Init()
{
	m_Born = new PlayerBorn;
	m_LeftArm = new PartsLeftArm;
	m_RightArm = new PartsRightArm;
	m_LeftArm->Init();
	m_RightArm->Init();
}

void PlayerRobot::Load()
{
	m_Born->Load();
	m_LeftArm->Load();
	m_RightArm->Load();
}

void PlayerRobot::Start()
{
	m_LeftArm->Start();
	m_RightArm->Start();
}

void PlayerRobot::Step()
{
	m_LeftArm->Step();
	m_RightArm->Step();
}

void PlayerRobot::Update()
{
	m_Born->Update();
	m_LeftArm->Update();
	m_RightArm->Update();
}

void PlayerRobot::Draw()
{
	m_Born->Draw();
	m_LeftArm->Draw();
	m_RightArm->Draw();
}

void PlayerRobot::Fin()
{
	m_Born->Fin();
	m_LeftArm->Fin();
	m_RightArm->Fin();
	delete m_Born;
	delete m_LeftArm;
	delete m_RightArm;
}
