#include "SkyImage.h"

SkyImage::SkyImage()
{
	m_Handle = 0;
	m_Pos = {};
}

SkyImage::~SkyImage()
{
	Fin();
}

void SkyImage::Load(const char* path)
{
	m_Handle = MV1LoadModel(path);
}

void SkyImage::Step()
{
}

void SkyImage::Update()
{
	MV1SetPosition(m_Handle, m_Pos);
	MV1SetRotationXYZ(m_Handle, m_Rot);
}

void SkyImage::Draw()
{
	MV1DrawModel(m_Handle);
}

void SkyImage::Fin()
{
	MV1DeleteModel(m_Handle);
}