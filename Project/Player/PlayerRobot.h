#pragma once

class PlayerBorn;
class PartsLeftArm;
class PartsRightArm;
class PartsLeg;
class PartsHeadBody;

class PlayerRobot
{
public:
	PlayerRobot();
	~PlayerRobot();
public:
	void Init();
	void Load();
	void Start();
	void Step();
	void Update();
	void Draw();
	void Fin();
private:
	PlayerBorn* m_Born;
	PartsRightArm* m_RightArm;
	PartsLeftArm* m_LeftArm;
	PartsLeg* m_Leg;
	PartsHeadBody* m_HeadBody;
};
