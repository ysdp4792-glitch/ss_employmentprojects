#include "DxLib.h"
#include "PlayScene.h"
#include "../Input/Input.h"
#include "../Player/PlayerManager.h"
#include "../Camera/CameraManager.h"
#include "../Stage/StageManager.h"
#include "../Bullet/BulletManager.h"
#include "../Collision/CollisionManager.h"
#include "../Parts/PartsManager.h"
#include "../Enemy/EnemyManager.h"
#include "../SkyImage/SkyImage.h"
#include "../Sound/Sound.h"

PlayScene::PlayScene() : SceneBase()
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Init()
{
	CollisionManager::CreateInstance();
	// プレイヤーマネージャーを生成
	PlayerManager::CreateInstance();
	PlayerManager* playerManager = PlayerManager::GetInstance();
	// プレイヤーを生成
	playerManager->CreatePlayer();
	// プレイヤーの初期化～開始
	playerManager->Init();

	// カメラマネージャーを生成
	CameraManager::CreateInstance();
	// カメラマネージャーを取得
	CameraManager* cameraManager = CameraManager::GetInstance();
	// カメラを生成
	cameraManager->CreateCamera(CAMERA);
	cameraManager->CreateCamera(DEBUG_CAMERA);
	// カメラの初期化
	cameraManager->Init();
	// ステージマネージャーを生成
	StageManager::CreateInstance();
	StageManager::GetInstance()->Init();

	// Ｚバッファを有効にする
	SetUseZBuffer3D(TRUE);

	// Ｚバッファへの書き込みを有効にする
	SetWriteZBuffer3D(TRUE);

	// 天球生成
	m_SkyImage = new SkyImage();

	// バレットマネージャーを生成
	BulletManager::CreateInstance();
	BulletManager* bulletManager = BulletManager::GetInstance();
	bulletManager->Init();

	PartsManager::CreateInstance();
	PartsManager* partsManager = PartsManager::GetInstance();
	partsManager->Init();

	EnemyManager::CreateInstance();
	EnemyManager* enemyManager = EnemyManager::GetInstance();
	enemyManager->Init();
}

void PlayScene::Load()
{
	// ステージをロード
	StageManager::GetInstance()->Load();

	// プレイヤーをロード
	PlayerManager::GetInstance()->Load();

	// カメラロード
	CameraManager::GetInstance()->Load();

	// バレットをロード
	BulletManager::GetInstance()->Load();

	PartsManager::GetInstance()->Load();

	EnemyManager::GetInstance()->Load();


	Sound::GetInstance()->Load();

	m_SkyImage->Load("Data/SkyImage/SkyImage.x");
}

void PlayScene::Start()
{
	// プレイヤー開始
	PlayerManager::GetInstance()->Start();

	// カメラ開始
	CameraManager::GetInstance()->Start();
	
	// バレット開始
	BulletManager::GetInstance()->Start();

	// デバッグ用に左腕パーツを作って表示
	PartsManager::GetInstance()->CreateParts(PLAYER_LEG); 
	PartsManager::GetInstance()->CreateParts(PLAYER_LEFT_ARM); 
	PartsManager::GetInstance()->CreateParts(PLAYER_RIGHT_ARM); 
	PartsManager::GetInstance()->CreateParts(PLAYER_HEAD_AND_BODY); 

	// パーツ開始
	PartsManager::GetInstance()->Start();

	EnemyManager::GetInstance()->Start();

	Sound::GetInstance()->PlayBGM("stage");
	Sound::GetInstance()->SetBGMVolume(100);
}

void PlayScene::Step()
{
	CameraManager* cameraManager = CameraManager::GetInstance();

	// デバッグカメラモード切り替え
	if (Input::IsTriggerKey(KEY_1))
	{
		CameraManager* cameraManager = CameraManager::GetInstance();
		// デバッグカメラON/OFF切り替え
		bool isDebugCamera = cameraManager->IsDebugCameraMode();
		if (isDebugCamera)
		{
			// デバッグカメラ解除
			cameraManager->ReleaseDebugCameraMode();
		}
		else
		{
			// デバッグカメラON
			cameraManager->ChangeDebugCameraMode();
		}
	}

	if (cameraManager->IsDebugCameraMode())
	{
		// デバッグカメラがONのときはカメラだけStep/Updateする
		cameraManager->Step();
		cameraManager->Update();
	}
	// デバッグカメラがOFFの時のみそれぞれのオブジェクトを動かす
	else
	{
		// プレイヤーステップ
		PlayerManager::GetInstance()->Step();
		CameraManager::GetInstance()->Step();
		BulletManager::GetInstance()->Step();
		PartsManager::GetInstance()->Step(); 
		CollisionManager::GetInstance()->CheckCollision();
		EnemyManager::GetInstance()->Step();
		m_SkyImage->Step();
	}
}

void PlayScene::Update()
{
	// プレイヤー更新
	PlayerManager::GetInstance()->Update();
	// カメラ更新
	CameraManager::GetInstance()->Update();

	BulletManager::GetInstance()->Update();

	PartsManager::GetInstance()->Update();

	EnemyManager::GetInstance()->Update();

	m_SkyImage->Update();
}

void PlayScene::Draw()
{
	// 天球を描画
	m_SkyImage->Draw();

	// ステージを描画
	StageManager::GetInstance()->Draw();
	// プレイヤー描画
	PlayerManager::GetInstance()->Draw();

	// カメラ描画
	CameraManager::GetInstance()->Draw();

	BulletManager::GetInstance()->Draw();

	PartsManager::GetInstance()->Draw();

	CollisionManager::GetInstance()->Draw();

	EnemyManager::GetInstance()->Draw();
}


void PlayScene::Fin()
{
    StageManager::DeleteInstance();
    PlayerManager::DeleteInstance();
    CameraManager::DeleteInstance();

    BulletManager::DeleteInstance();
    PartsManager::DeleteInstance();

    // 依存オブジェクト（Enemy）が先に破棄されるようにする
    EnemyManager::DeleteInstance();

    // 後で CollisionManager を破棄
    CollisionManager::DeleteInstance();

    delete m_SkyImage;
}
