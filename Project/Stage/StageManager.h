#pragma once

class Stage;

class StageManager
{
public:
	StageManager();
	~StageManager();

public:
	static void CreateInstance() { if (!m_Instance)m_Instance = new StageManager; }
	static StageManager* GetInstance() { return m_Instance; }
	static void DeleteInstance() { if (m_Instance) delete m_Instance; m_Instance = nullptr; }

public:
	void Init();
	void Load();
	void Draw();
	void Fin();

	Stage* GetStage() const { return m_Stage; }

private:
	static StageManager* m_Instance;
	Stage* m_Stage;
};
