//===========================================================================================================================================================
// 
// ベースオブジェクトのウィンドウ [edit_window_baseobj.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "edit_window_baseobj.h"
#include "ImGUI_manager.h"
#include "manager.h"
#include "calculation.h"

//===========================================================================================================
// コンストラクタ
//===========================================================================================================
EditWindowBaseobj::EditWindowBaseobj()
{
	m_MoveNum = 1;
	m_move = { 0,0,0 };
	m_IsOpenAddtypeWindow = true;
}

//===========================================================================================================
// デストラクタ
//===========================================================================================================
EditWindowBaseobj::~EditWindowBaseobj()
{
}

//===========================================================================================================
// 始動
//===========================================================================================================
void EditWindowBaseobj::Begin()
{
	ImGui::Begin("BaseObject Editer");
}

//===========================================================================================================
// 終了
//===========================================================================================================
void EditWindowBaseobj::End()
{
	ImGui::End();
}

//===========================================================================================================
// タイプ選択
//===========================================================================================================
int EditWindowBaseobj::SelectType(int& type)
{
	static int i = 0;

	// モデルファイルパスの情報の取得
	int MaxIdx = CManager::GetModeldata()->GetMaxFilepassIdx();
	CModeldata::MODELFILEPASS* modelfilepass = CManager::GetModeldata()->GetModelFilePassData();
	const char* DisplayName[128] = {};	// 表示名を格納する変数

	// 表示名を設定する
	for (int n = 0; n <= MaxIdx; n++)
	{
		DisplayName[n] = modelfilepass[n].Name.c_str();
	}

	if (ImGui::CollapsingHeader("TYPE"))
	{
		// オブジェクトリスト
		if (ImGui::ListBox("type", &i, DisplayName, (MaxIdx+1), 4))
		{
			type = i + 1;
		}

		// ファイル追加見出し
		ImGui::SeparatorText("AddFile");

		// モデルファイル追加ウィンドウ
		DrawAddModelFileWindow();
	}

	return type;
}

//===========================================================================================================
// 移動
//===========================================================================================================
int EditWindowBaseobj::MoveBar(D3DXVECTOR3& move)
{
	ImGui::SliderInt("max move", &m_MoveNum, 1, m_MaxMoveSlider);

	// スライダーより大きい数値を入力してしまったとき
	if (m_MoveNum > m_MaxMoveSlider)
	{
		m_MoveNum = m_MaxMoveSlider;
	}

	// 0 以下を入力してしまった場合
	else if (m_MoveNum < 1)
	{
		m_MoveNum = 1;
	}

	// ドラッグで位置を動かす
	ImGui::DragFloat3(
		"drag float3",
		move,
		1.f,
		-(float)m_MoveNum,
		(float)m_MoveNum,
		"%.3f",
		ImGuiSliderFlags_NoInput
	);

	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		for (int i = 0; i < 3; i++)
		{
			move = { 0,0,0 };
		}
	}

	return m_MoveNum;
}

//===========================================================================================================
// 位置を直接入力する
//===========================================================================================================
void EditWindowBaseobj::PosNum(D3DXVECTOR3& pos)
{
	// pos を直接入れる箱
	ImGui::InputFloat("pos X", &pos.x, (float)m_MoveNum, 0, "%.3f");
	ImGui::InputFloat("pos Y", &pos.y, (float)m_MoveNum, 0, "%.3f");
	ImGui::InputFloat("pos Z", &pos.z, (float)m_MoveNum, 0, "%.3f");
}

//===========================================================================================================
// 拡大率を直接入力する
//===========================================================================================================
void EditWindowBaseobj::ScaleNum(D3DXVECTOR3& scale)
{
	if (ImGui::CollapsingHeader("SCALE"))
	{// 移動をすべてまとめたヘッダー

		// scale を直接入れる箱
		ImGui::InputFloat("scale X", &scale.x, 0.1f, 0, "%.1f");
		ImGui::InputFloat("scale Y", &scale.y, 0.1f, 0, "%.1f");
		ImGui::InputFloat("scale Z", &scale.z, 0.1f, 0, "%.1f");
	}

	// マイナスを下回る時
	if (scale.x <= 0.0f)
	{
		scale.x = 0.1f;
	}
	if (scale.y <= 0.0f)
	{		  
		scale.y = 0.1f;
	}
	if (scale.z <= 0.0f)
	{
		scale.z = 0.1f;
	}
}

//===========================================================================================================
// 角度を直接入力する
//===========================================================================================================
void EditWindowBaseobj::RotNum(D3DXVECTOR3& rot)
{
	if (ImGui::CollapsingHeader("ROT"))
	{// 移動をすべてまとめたヘッダー

		// rot を直接入れる箱
		ImGui::InputFloat("rot X", &rot.x, 0.1f, 0, "%.2f");
		ImGui::InputFloat("rot Y", &rot.y, 0.1f, 0, "%.2f");
		ImGui::InputFloat("rot Z", &rot.z, 0.1f, 0, "%.2f");
	}

	// カメラ角度リセット
	CCalculation::ResetRot(rot.x);
	CCalculation::ResetRot(rot.y);
	CCalculation::ResetRot(rot.z);

	//// カメラが一周すると角度をリセットする
	//if (rot.y > D3DX_PI /*|| rot.y <= -D3DX_PI*/)
	//{
	//	rot.y = -D3DX_PI;
	//}
	//if (rot.y < D3DX_PI)
	//{

	//}
}

//===========================================================================================================
// モデルファイルの追加ウィンドウ
//===========================================================================================================
void EditWindowBaseobj::DrawAddModelFileWindow()
{
	static char IdName[128] = "ID NAME";	// モデルの名前
	char *filepass[128];	// ファイルパス

	filepass[0] = ImguiManager::GetEditMain()->GetDropFilename();	// ファイルパスを取得する

	ImGui::InputText("file name", filepass[0], IM_ARRAYSIZE(filepass));	// ファイルパスを表示

	ImGui::InputText("ID_NAME", IdName, IM_ARRAYSIZE(IdName));	// 名前入力

	if (ImGui::Button("ADD", { 200,20 }))	// 追加ボタン
	{
		// モデルファイルパスデータを書き出す
		m_AddResult = CManager::GetModeldata()->SetModelFilepass(IdName, filepass[0],CModeldata::ADD_DATA_WRITING);
	}

	ImGui::Text(m_AddResult.c_str());	// 結果をテキストに表示する
}
