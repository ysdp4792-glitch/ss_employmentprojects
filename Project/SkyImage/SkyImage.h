#pragma once
#include "DxLib.h"

class SkyImage
{
public:
	SkyImage();
	~SkyImage();

public:
	void Load(const char* path);
	void Step();
	void Update();
	void Draw();
	void Fin();

private:
	int m_Handle;
	VECTOR m_Pos;
	VECTOR m_Rot;
};