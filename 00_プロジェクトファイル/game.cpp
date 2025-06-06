//===========================================================================================================================================================
// 
// ゲームの処理
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "game.h"
#include "renderer.h"
#include "UI.h"
#include "sound.h"

#include "player3D.h"
#include "block3D.h"
#include "enemy3D.h"
#include "enemyland3D.h"
#include "grass3D.h"
#include "input.h"
#include "manager.h"
#include "slime_enemy.h"
#include "load.h"
#include "golem_enemy.h"
#include "search.h"
#include "slime_boss.h"
#include "block_gimmick.h"

int CGame::m_nDelayEnd = 0; // 倒してからリザルトになるまで
bool CGame::m_Delay = false;
CFragmentManager* CGame::m_pFeagmentMG = nullptr;
//===========================================================================================================
// コンストラクタ
//===========================================================================================================
CGame::CGame()
{
}

//===========================================================================================================
// デストラクタ
//===========================================================================================================
CGame::~CGame()
{
}

//===========================================================================================================
// 初期設定
//===========================================================================================================
HRESULT CGame::Init()
{
	m_pFeagmentMG = new CFragmentManager;

	CScene::Init();

	CLoad::LoadText();

	for (int i = 0; i < 30; i++)
	{
		CBlockGimmick::Create({ 770.0f,0.0f + (10.0f * i),0.0f });
		CBlockGimmick::Create({ 1010.0f,0.0f + (10.0f * i),0.0f });
	}

	//CGolemEnemy::Create({ 150.0f,60.0f,0.0f });
	//CGolemEnemy::Create({ 390.0f,60.0f,0.0f });

	CSlimeEnemy::Create({ 120.0f,60.0f,0.0f });
	CSlimeEnemy::Create({ 250.0f,30.0f,0.0f });
	CSlimeEnemy::Create({ 250.0f,81.0f,0.0f });
	
	CPlayer3D::Create({ 40.0f, 25.0f, 0.0f });

	CSlimeBoss::Create({ 920.0f,0.0f,0.0f });

	SetInitUI();

	PlaySound(SOUND_LABEL_GAME_BGM);

	//CBlock3D::Create({ 15.f * 18.f,-100 + (18.f*2.0f),0.f });
	//CBlock3D::Create({ 0.0f,10.0f,0.0f });

	return S_OK;
}

//===========================================================================================================
// 初期UIのセット
//===========================================================================================================
void CGame::SetInitUI()
{
	//CUI::Create(
	//	{ SCREEN_WIDTH * 0.4f,SCREEN_HEIGHT * 0.78f,0.0f }
	//	, { 50.0f,50.0f,0 }
	//	, -1
	//	, "data\\TEXTURE\\ui_key_square_1_000.png"
	//);
	//CUI::Create(
	//	{ SCREEN_WIDTH * 0.6f,SCREEN_HEIGHT * 0.78f,0.0f }
	//	, { 50.0f,50.0f,0 }
	//	, -1
	//	, "data\\TEXTURE\\ui_key_square_2_000.png"
	//);
}

//===========================================================================================================
// 終了処理
//===========================================================================================================
void CGame::Uninit()
{
	CScene::Uninit();

	if (m_pFeagmentMG != nullptr)
	{
		delete m_pFeagmentMG;
		m_pFeagmentMG = nullptr;
	}

	StopSound(SOUND_LABEL_GAME_BGM);

	//if (m_pText != nullptr)
	//{
	//	m_pText->Uninit();
	//	delete m_pText;
	//	m_pText = nullptr;
	//}
}

void CGame::UpdateDelay()
{
	
}

//===========================================================================================================
// 更新処理
//===========================================================================================================
void CGame::Update()
{
	

	CInputKeyBoard* keyboard = CManager::GetKeyboard();
	CFade* pFade = CManager::GetFade();

	// プレイヤーを検索
	CPlayer3D* pPlayer = nullptr;
	pPlayer = CSearch::SearchObject(pPlayer, CObject::TYPE::PLAYER);

	std::vector<CSlimeBoss*>pSlimeBoss{};
	pSlimeBoss = CSearch::SearchMultiObject(pSlimeBoss, CObject::TYPE::ENEMY_BOSS);

	if (pPlayer != nullptr)
	{
		if (pPlayer->GetPos().x >= 780.0f)
		{
			for (unsigned int n = 0; n < pSlimeBoss.size(); n++)
			{
				if (pSlimeBoss[n]->GetTag() == CEnemy3D::TAG_ENEMY::TAG_SLIME)
				{
					pSlimeBoss[n]->SetBossAppear(true);
				}
			}
		}
		else
		{
			for (unsigned int n = 0; n < pSlimeBoss.size(); n++)
			{
				pSlimeBoss[n]->SetBossAppear(false);
			}
		}

		for (unsigned int n = 0; n < pSlimeBoss.size(); n++)
		{
			if (pSlimeBoss[n]->GetTag() == CEnemy3D::TAG_ENEMY::TAG_SLIME)
			{
				if (pSlimeBoss[n]->GetLife() <= 0)
				{
					m_Delay = true;
					
				}
			}
		}
	}

	if (m_Delay)
	{
		if (m_nDelayEnd >= 0)
		{
			if (m_nDelayEnd == 0)
			{
				m_Delay = false;
				pFade->SetFade(CScene::MODE_RESULT);
			}
			m_nDelayEnd--;
		}
	}
	


#ifdef _DEBUG
	

	// デバッグ用右矢印でシーン切り替え
	if (keyboard->GetTrigger(DIK_RIGHTARROW))
	{
		pFade->SetFade(CScene::MODE_TITLE);

		//CManager::SetMode(CScene::MODE_GAME);
	}

	if (keyboard->GetTrigger(DIK_F2))
	{
		pFade->SetFade(CScene::MODE_EDITOR);
	}

#endif // _DEBUG

	CScene::Update();
}

//===========================================================================================================
// 描画処理
//===========================================================================================================
void CGame::Draw()
{
	CScene::Draw();
}