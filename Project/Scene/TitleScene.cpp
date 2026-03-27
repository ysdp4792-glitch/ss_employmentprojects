#include "DxLib.h"
#include "TitleScene.h"
#include "../Scene/SceneManager.h"
#include "../Sound/Sound.h"
#include "../Input/Input.h"

static int s_TitleBgmHandle = -1; // 追加: ファイルをロードするハンドル

TitleScene::TitleScene() : SceneBase()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
}

void TitleScene::Load()
{
	m_Handle = LoadGraph("Data/Title/TitleImage.png");
	m_TitleHandle = LoadGraph("Data/Title/TitleFont.png");
	Sound::GetInstance()->Load();
}

void TitleScene::Start()
{
	Sound::GetInstance()->PlayBGM("title");
	Sound::GetInstance()->SetBGMVolume(100);
}
void TitleScene::Step()
{
	// Zキーでプレイシーンへ
	if (Input::IsTriggerKey(KEY_Z))
	{
		SceneManager::GetInstance()->ChangeScene(PLAY);
	}
}

void TitleScene::Update()
{
}

void TitleScene::Draw()
{
	// 背景を描画
	DrawGraph(0, 0, m_Handle, TRUE);

	// タイトル画像を縮小して描画（左上基準）
	if (m_TitleHandle >= 0)
	{
		int srcW = 0, srcH = 0;
		GetGraphSize(m_TitleHandle, &srcW, &srcH);

		// サイズ調整（0.0f - 1.0f）。好みに応じて変更してください。
		const float scale = 0.8f;

		int dstW = (int)(srcW * scale);
		int dstH = (int)(srcH * scale);

		// 左上 (x=0,y=0) に縮小して描画
		DrawExtendGraph(0, 0, dstW, dstH, m_TitleHandle, TRUE);
	}
}

void TitleScene::Fin()
{
	// タイトル終了時に停止・解放
	if (s_TitleBgmHandle >= 0)
	{
		StopSoundMem(s_TitleBgmHandle);
		DeleteSoundMem(s_TitleBgmHandle);
		s_TitleBgmHandle = -1;
	}
}
