//===========================================================================================================================================================
// 
// ステージ編集ツールの場面
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "manager.h"
#include "search.h"
#include "input.h"
#include "camera.h"
#include "editor.h"
#include "fade.h"

#include "player3D.h"
#include "grass3D.h"
#include "slime_enemy.h"
#include "stone.h"
#include "start.h"
#include "goal.h"
#include "slime_enemy.h"
#include "golem_enemy.h"

#include "load.h"
#include "save.h"

#include "nlohmann/test.hpp"

#include < fstream >

// 静的メンバ変数の初期化
CModel* CEditor::pBaseObj = nullptr;
int CEditor::m_type = CModel::MODEL_TYPE::NORMAL;
int CEditor::m_old_type = CModel::MODEL_TYPE::NORMAL;

//===========================================================================================================
// コンストラクタ
//===========================================================================================================
CEditor::CEditor()
{
	IsOpenBaseObjWindow = false;
}

//===========================================================================================================
// デストラクタ
//===========================================================================================================
CEditor::~CEditor()
{
}

//===========================================================================================================
// 初期設定
//===========================================================================================================
HRESULT CEditor::Init()
{

	//std::ifstream stream("nlohmann\\test.hpp");

	//if(!stream.is_open())
	//	throw new std::exception("Failed open file.");

	//if (!json::accept(stream))
	//	throw new std::exception("jsonのフォーマットが不正");

	//// json::acceptがフォーマットチェック時にpositionを進めてしまうので、先頭に戻す
	//stream.seekg(0, std::ios::beg);

	//json j = json::parse(stream);

	//auto result = j.get<test>();

	pBaseObj = new CModel;

	pBaseObj->SetPos({ 0.0f,0.0f,0.0f });
	pBaseObj->SetModel(CModel::MODEL_TYPE::GRASS);
	pBaseObj->SetType(CObject::BASE);
	pBaseObj->Init();

	// モデルタイプの設定
	m_type = CModel::MODEL_TYPE::GRASS;
	m_old_type = m_type;	// 現在と古いタイプを統一させる

	// モデルファイルデータの読み込み

	CLoad::LoadText();
	CManager::GetModeldata()->LoadModelFilePass();	// モデルファイルパス
	
	CScene::Init();

	return S_OK;
}

//===========================================================================================================
// 終了処理
//===========================================================================================================
void CEditor::Uninit()
{
	if (pBaseObj != nullptr)
	{
		pBaseObj->Uninit();
	}

	CScene::Uninit();
}

//===========================================================================================================
// 更新処理
//===========================================================================================================
void CEditor::Update()
{
	CInputKeyBoard* keyboard = CManager::GetKeyboard();
	CFade* pFade = CManager::GetFade();

	D3DXVECTOR3 pos = pBaseObj->GetPos();

	UpdateSetModel(m_type);
	
	InputEdit(pos);
	
	pBaseObj->SetPos(pos);

	CCamera* pCamera = CManager::GetCamera();	// camera を持ってくる
	pCamera->SetTargetPos(pos);			// camera に object を入れる
	
	CScene::Update();

	if (keyboard->GetTrigger(DIK_F3))
	{
		pFade->SetFade(CScene::MODE_GAME);
	}
}

//===========================================================================================================
// 描画処理
//===========================================================================================================
void CEditor::Draw()
{
	CScene::Draw();
}

//===========================================================================================================
// エディターの入力処理
//===========================================================================================================
void CEditor::InputEdit(D3DXVECTOR3 &pos)
{
	CFade* pFade = CManager::GetFade();

	// ウィンドウの動作を受け取る変数
	EditWindowMain::WindowAction w_act = EditWindowMain::W_ACT_NONE;

	// Edit Main をもってくる
	EditWindowMain* pEditmain = CManager::GetImguiManager()->GetEditMain();
	w_act = pEditmain->DrawWindow();	// MainWindowの描画

	CInputKeyBoard* keyboard = CManager::GetKeyboard();	// キーボードの取得

	// ステージ情報を書き出す
	if (keyboard->GetTrigger(DIK_F1) || w_act == EditWindowMain::W_ACT_SAVE)
	{// F1が押されたとき、または ImGUI のクリエイトボタンが押されたとき

		CSave::StageText();
	}

	// ステージ情報を読み込む
	if (w_act == EditWindowMain::W_ACT_LOAD)
	{// ImGUI のロードボタンが押されたとき
		pFade->SetFade(CScene::MODE_EDITOR);
	}
	
	if (w_act == EditWindowMain::W_ACT_OPEN_BLOCK)
	{
		IsOpenBaseObjWindow = true;
	}

	if (IsOpenBaseObjWindow)
	{
		InputBaseObj(pos);
	}

	SelectObject();	// オブジェクトを選択

	// オブジェクト生成
	if (keyboard->GetTrigger(DIK_RETURN))
	{
		CreateEdit(pos);
	}
	// オブジェクト削除
	else if (keyboard->GetTrigger(DIK_BACKSPACE))
	{
		DestroyEdit(pos);
	}

	

#ifdef _DEBUG
#endif // _DEBUG

	//return pos;
}

//===========================================================================================================
// ベースオブジェクトの操作
//===========================================================================================================
void CEditor::InputBaseObj(D3DXVECTOR3& pos)
{
	// ベースオブジェクトを動かすウィンドウを取得
	EditWindowBaseobj* pEditBaseobj = CManager::GetImguiManager()->GetEditBase();

	D3DXVECTOR3 scale = pBaseObj->GetScale();
	D3DXVECTOR3 rot = pBaseObj->GetRot();
	
	pEditBaseobj->Begin();	// 初期化

	pEditBaseobj->SelectType(m_type);	// ベースオブジェクトの種類を選択

	InputMove(pos);	// 移動関連

	pEditBaseobj->ScaleNum(scale);

	pEditBaseobj->RotNum(rot);

	pBaseObj->SetRot(rot);
	pBaseObj->SetScale(scale);

	//配置物生成ボタン
	if (ImGui::Button("create"))
		CreateEdit(pos);

	// スペースを開ける
	ImGui::SameLine(0, 10);

	// 配置物削除ボタン
	if (ImGui::Button("destroy"))
		DestroyEdit(pos);

	pEditBaseobj->End();	// 窓の終了
}

//===========================================================================================================
// 移動操作
//===========================================================================================================
void CEditor::InputMove(D3DXVECTOR3& pos)
{
	// ベースオブジェクトを動かすウィンドウを取得
	EditWindowBaseobj* pEditBaseobj = CManager::GetImguiManager()->GetEditBase();

	int MoveMag = 1;	// 移動倍率
	D3DXVECTOR3 move = { 0,0,0 };	// 移動値

	if (ImGui::CollapsingHeader("MOVE"))
	{// 移動をすべてまとめたヘッダー

		// bar で BaseObject 移動
		MoveMag = pEditBaseobj->MoveBar(move);
		pos += move;

		// pos 直接入力
		pEditBaseobj->PosNum(pos);
	}

	// キーボード移動操作
	InputKeyboadMove(pos, MoveMag);
}

//===========================================================================================================
// ベースオブジェクトのキーボード操作
//===========================================================================================================
void CEditor::InputKeyboadMove(D3DXVECTOR3& pos,int mag)
{
	CInputKeyBoard* keyboard = CManager::GetKeyboard();
	D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };

	// 移動方向を決める
	if (keyboard->GetTrigger(DIK_D))
	{
		move.x = 1;
	}
	else if (keyboard->GetTrigger(DIK_A))
	{
		move.x = -1;
	}
	else if (keyboard->GetTrigger(DIK_W))
	{
		move.y = 1;
	}
	else if (keyboard->GetTrigger(DIK_S))
	{
		move.y = -1;
	}

	// move に mag(移動倍率)をかけて移動させる
	pos += move * mag;
}

//===========================================================================================================
// オブジェクト選択する
//===========================================================================================================
void CEditor::SelectObject()
{
	CInputKeyBoard* keyboard = CManager::GetKeyboard();

	// タイプを選択
	if (keyboard->GetTrigger(DIK_E))
	{// タイプを次に進める
		m_type++;
	}
	else if (keyboard->GetTrigger(DIK_Q))
	{// タイプを前に戻す
		m_type--;
	}

	// タイプが既定の数を超えてしまった場合
	if (m_type == CModel::MODEL_TYPE::MAX)
	{// タイプが最大以上になった時
		m_type = CModel::MODEL_TYPE::NORMAL + 1;	// 最初のタイプに戻す
	}
	if (m_type == CModel::MODEL_TYPE::NORMAL)
	{// タイプが最小以下になった時
		m_type = CModel::MODEL_TYPE::MAX - 1;	// 最後のタイプに戻す
	}
}

//===========================================================================================================
// タイプごとに生成する
//===========================================================================================================
void CEditor::CreateEdit(const D3DXVECTOR3 pos)
{
	typeid(pBaseObj).name();

	//CObject* pObj = nullptr;

	switch (m_type)
	{
	case CModel::MODEL_TYPE::GRASS:
		CGrass::Create(pos);
		break;

	case CModel::MODEL_TYPE::STONE:
		CStone::Create(pos);
		break;

	case CModel::MODEL_TYPE::START:
		CStart::Create(pos);
		break;

	case CModel::MODEL_TYPE::GOAL:
		CGoal::Create(pos);
		break;

	case CModel::MODEL_TYPE::SLIME:
		CSlimeEnemy::Create(pos);
		break;

	case CModel::MODEL_TYPE::GOLEM:
		CGolemEnemy::Create(pos);
		break;
	}
}

//===========================================================================================================
// オブジェクトを破棄する
//===========================================================================================================
void CEditor::DestroyEdit(const D3DXVECTOR3 pos)
{
	// 変数宣言
	CModel* pModel = nullptr;	// モデルポインタの初期化

	// 全オブジェクトの検索
	for (int nPrio = 0; nPrio < CObject::PRIORITY_MAX; nPrio++)
	{
		CObject* pObj = CObject::GetTop(nPrio);	// オブジェクトの最初を設定する
		while (pObj != nullptr)
		{// オブジェクトが存在する限り

			CObject* pNext = pObj->GetNext();	// 次のオブジェクトを設定する
			pModel = CSearch::SearchType(pModel, pObj);	// タイプをセットする

			if (pos == pModel->GetPos() && pModel->GetType() != CObject::BASE)
			{// 位置が一致、ベースオブジェクト以外
				pModel->Uninit();	// 削除
				break;
			}
			// ポインタを次のオブジェクトに設定する
			pObj = pNext;
		}
	}
}

//===========================================================================================================
// ベースの見た目を変える
//===========================================================================================================
void CEditor::UpdateSetModel(int type)
{
	// タイプが変わっていなかったらこの処理を通さない
	if (m_type == m_old_type)
		return;

	// モデルの見た目の設定する
	CModeldata::MODELFILEPASS* filepassdata = CManager::GetModeldata()->GetModelFilePassData();
	pBaseObj->SetModel(filepassdata[type-1].Filepass.c_str());

	m_old_type = m_type;	// タイプを一致させる

	//pBaseObj->SetModel((CModel::MODEL_TYPE)type);

}

