//===========================================================================================================================================================
// 
// タイトル背景 [titleBG.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "resultBG.h"
#include "texture.h"
#include "manager.h"

//========================================================================================================================
// コンストラクタ
//========================================================================================================================
CResultBG::CResultBG(int nPriority) : CObject2D(nPriority)
{
}

//========================================================================================================================
// デストラクタ
//========================================================================================================================
CResultBG::~CResultBG()
{
}

//========================================================================================================================
// 初期設定
//========================================================================================================================
HRESULT CResultBG::Init()
{
	int nIdx = 0;

	// テクスチャの設定
	CTexture* pTexData = CManager::GetTexture();
	nIdx = pTexData->Regist("data\\TEXTURE\\resultBG_000.png");
	BindTexture(pTexData->GetAddress(nIdx));

	SetSize({ SCREEN_WIDTH ,SCREEN_HEIGHT ,0.0f });	// サイズの設定
	SetPos({ SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f });	// 位置の設定

	CObject2D::Init();	// 2D初期化処理

	return S_OK;
}

//========================================================================================================================
// 終了処理
//========================================================================================================================
void CResultBG::Uninit()
{
	CObject2D::Uninit();
}

//========================================================================================================================
// 更新処理
//========================================================================================================================
void CResultBG::Update()
{
	CObject2D::Update();
}

//========================================================================================================================
// 描画処理
//========================================================================================================================
void CResultBG::Draw()
{
	CObject2D::Draw();
}

//========================================================================================================================
// 生成処理
//========================================================================================================================
CResultBG* CResultBG::Create()
{
	// リザルト背景
	CResultBG* pResultBG = new CResultBG;

	// 初期化
	pResultBG->Init();

	return pResultBG;
}