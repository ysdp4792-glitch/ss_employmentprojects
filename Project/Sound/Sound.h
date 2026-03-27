#pragma once
#include <string>
#include <unordered_map>

class Sound {
public:
    static Sound* GetInstance();

    void Init();
    void Load();
    void PlayBGM(const std::string& name, bool loop = true);
    void StopBGM();
    void PlaySE(const std::string& name);
    void SetBGMVolume(int volume); // 0〜255
    void SetSEVolume(int volume);  // 0〜255
    void Fin();

private:
    Sound();
    ~Sound();

    static Sound* m_Instance;

    std::unordered_map<std::string, int> m_BGMHandles;
    std::unordered_map<std::string, int> m_SEHandles;

    int m_CurrentBGMHandle;
    int m_BGMVolume;
    int m_SEVolume;
};
