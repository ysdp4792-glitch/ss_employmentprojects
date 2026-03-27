#include "Sound.h"
#include <DxLib.h>
#include "../Born/PlayerBorn.h"

Sound* Sound::m_Instance = nullptr;

Sound* Sound::GetInstance() {
    if (!m_Instance) m_Instance = new Sound();
    return m_Instance;
}

Sound::Sound() : m_CurrentBGMHandle(-1), m_BGMVolume(255), m_SEVolume(255) {}

Sound::~Sound() {
    Fin();
}

void Sound::Init() {
    m_BGMHandles.clear();
    m_SEHandles.clear();
}

void Sound::Load() {
    // BGM
    m_BGMHandles["title"] = LoadSoundMem("Data/Sound/BGM/TitleBGM.mp3");
    m_BGMHandles["stage"] = LoadSoundMem("Data/Sound/BGM/StageBGM.mp3");
    m_BGMHandles["clear"] = LoadSoundMem("Data/Sound/BGM/ClearBGM.mp3");

    // SE
    m_SEHandles["shot"] = LoadSoundMem("Data/Sound/SE/shot.wav");
    m_SEHandles["hit"] = LoadSoundMem("Data/Sound/SE/hit.wav");
}

void Sound::PlayBGM(const std::string& name, bool loop) {
    StopBGM();
    auto it = m_BGMHandles.find(name);
    if (it != m_BGMHandles.end()) {
        m_CurrentBGMHandle = it->second;
        ChangeVolumeSoundMem(m_BGMVolume, m_CurrentBGMHandle);
        PlaySoundMem(m_CurrentBGMHandle, loop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK);
    }
}

void Sound::StopBGM() {
    if (m_CurrentBGMHandle >= 0) {
        StopSoundMem(m_CurrentBGMHandle);
        m_CurrentBGMHandle = -1;
    }
}

void Sound::PlaySE(const std::string& name) {
    auto it = m_SEHandles.find(name);
    if (it != m_SEHandles.end()) {
        ChangeVolumeSoundMem(m_SEVolume, it->second);
        PlaySoundMem(it->second, DX_PLAYTYPE_BACK);
    }
}

void Sound::SetBGMVolume(int volume) {
    m_BGMVolume = volume;
    if (m_CurrentBGMHandle >= 0) {
        ChangeVolumeSoundMem(m_BGMVolume, m_CurrentBGMHandle);
    }
}

void Sound::SetSEVolume(int volume) {
    m_SEVolume = volume;
}

void Sound::Fin() {
    for (auto it = m_BGMHandles.begin(); it != m_BGMHandles.end(); ++it) {
        DeleteSoundMem(it->second);
    }
    for (auto it = m_SEHandles.begin(); it != m_SEHandles.end(); ++it) {
        DeleteSoundMem(it->second);
    }
    m_BGMHandles.clear();
    m_SEHandles.clear();
}
