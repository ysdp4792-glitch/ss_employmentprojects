#include "EffekseerForDXLib.h"
#include "EffecseerEffect.h"

EffekseerEffect::EffekseerEffect()
{
	m_Active = false;
	m_Handle = 0;
	m_Pos = VGet(0.0f, 0.0f, 0.0f);
}

EffekseerEffect::~EffekseerEffect()
{
	Fin();
}

void EffekseerEffect::Step()
{
	// çƒê∂íÜÇ©Ç«Ç§Ç©
	if (IsEffekseer3DEffectPlaying(m_Handle) != 0)
	{
		m_Active = false;
	}
}

void EffekseerEffect::Update()
{
	if (!m_Active) return;

	// à íuê›íË
	SetPosPlayingEffekseer3DEffect(m_Handle, m_Pos.x, m_Pos.y, m_Pos.z);
}

void EffekseerEffect::Fin()
{
}

void EffekseerEffect::Play(int handle)
{
	m_Handle = PlayEffekseer3DEffect(handle);
}

void EffekseerEffect::Stop()
{
	StopEffekseer3DEffect(m_Handle);
	m_Active = false;
}
