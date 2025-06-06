//===========================================================================================================================================================
// 
// タイトルの処理
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "title.h"
#include "player3D.h"
#include "grass3D.h"
#include "input.h"
#include "manager.h"
#include "titleBG.h"
#include "sound.h"

CTitle::CTitle()
{
}

CTitle::~CTitle()
{
}

HRESULT CTitle::Init()
{
	CScene::Init();

	CTitleBG::Create();

	CText* pText = CRenderer::GetText();
	pText->SetText(CText::TEXT_PRESSENTER);

	PlaySound(SOUND_LABEL_TITLE_BGM);

	//CPlayer3D::Create({ 0.0f, 0.0f, 0.0f });

	//CGrass::Create({ 0.0f, -50.0f, 0.0f });

	return S_OK;
}

void CTitle::Uninit()
{
	// テキストを非表示にする
	CText* pText = CRenderer::GetText();
	pText->UnSetText(CText::TEXT_PRESSENTER);

	StopSound(SOUND_LABEL_TITLE_BGM);


	CScene::Uninit();
}

void CTitle::Update()
{
	CScene::Update();

	CInputKeyBoard* keyboard = CManager::GetKeyboard();
	CInputJoypad* joypad = CManager::GetJoypad();

	if (keyboard->GetTrigger(DIK_RETURN) || joypad->GetTrigger(CInputJoypad::JOYKEY_START))
	{
		CFade* pFade = CManager::GetFade();

		pFade->SetFade(CScene::MODE_GAME);

		//CManager::SetMode(CScene::MODE_GAME);
	}
}

void CTitle::Draw()
{
	CScene::Draw();
}