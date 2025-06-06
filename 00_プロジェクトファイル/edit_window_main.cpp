//===========================================================================================================================================================
// 
// メインのエディットウィンドウ [edit_window_main.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "edit_window_main.h"
#include "ImGUI_manager.h"
#include "manager.h"

float vec4f[3] = { 0,0,0 };


//===========================================================================================================
// コンストラクタ
//===========================================================================================================
EditWindowMain::EditWindowMain()
{
	strcpy(m_Filepass, "null");
}

//===========================================================================================================
// デストラクタ
//===========================================================================================================
EditWindowMain::~EditWindowMain()
{
}

//===========================================================================================================
// ウィンドウの表示
//===========================================================================================================
EditWindowMain::WindowAction EditWindowMain::DrawWindow()
{
	// ウィンドウがどのような動作をしたか返す変数
	WindowAction w_act = W_ACT_NONE;

	// 小ウィンドウの名前
	const char* window_name[128] = {};

	// IOの取得
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	const char* listbox_menu[] = { "block" };	// メニューのリスㇳ
	int listbox_menu_current = -1;		// メニューの番号
	
	// main window
	{
		ImGui::Begin("main window");	// main window を開く

		//if (&m_Filepass != nullptr)
		{
			CModeldata::MODELFILEPASS* pModeldata = nullptr;
			pModeldata = CManager::GetModeldata()->GetModelFilePassData();

			ImGui::Text(pModeldata[0].Filepass.c_str());
		}


		// アイテムリスト
		ImGui::ListBox("ItemList", &listbox_menu_current, listbox_menu, IM_ARRAYSIZE(listbox_menu), 4);

		// 書き出しボタン
		if (ImGui::Button("SAVE", { 80,30 })) 
		{ w_act = W_ACT_SAVE; }		// 動作の状態を save に変える

		ImGui::SameLine();

		// 書き出しボタン
		if (ImGui::Button("LOAD", { 80,30 }))
		{	w_act = W_ACT_LOAD;	}		// 動作の状態を load に変える

		// フレームレート表示
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

		ImGui::End();
	}
	

	if (listbox_menu_current != WINDOWTYPE_NULL)
	{	
		// クリックしたリストによって開くウィンドウを変える
		switch (listbox_menu_current)
		{
		case WINDOWTYPE_BLOCKLLIST:
			w_act = W_ACT_OPEN_BLOCK;
			break;
		}

		//// ウィンドウ名変更
		//ChangeWindowName(&window_name[0], listbox_menu_current);

		//// 新しいウィンドウを生成
		//ImGui::Begin(window_name[0]);

		//// ウィンドウの中身
		//WindowContents(&listbox_menu_current);

		//// ウィンドウの破棄
		//ImGui::End();
	}

	return w_act;	// 動作の状態を返す
}

//===========================================================================================================
// ウィンドウ名変更
//===========================================================================================================
void EditWindowMain::ChangeWindowName(const char* str[128],int current)
{
	// 選択されたリストによって名前を変更する
	switch (current)
	{
	case WINDOWTYPE_NULL:	// null
		str[0] = "null";
		break;

	case WINDOWTYPE_BLOCKLLIST:	// block
		str[0] = "block list";
		break;
	}
}

//===========================================================================================================
// ウィンドウの中身
//===========================================================================================================
void EditWindowMain::WindowContents(int *current)
{
	switch (*current)
	{
	case WINDOWTYPE_NULL:
		
		break;
	case WINDOWTYPE_BLOCKLLIST:
		BlockList();
		break;
	}
	

	if (ImGui::Button("create", { 70,30 }))
	{

	}
	
	// スペースを開ける
	ImGui::SameLine(0,30);

	// ウィンドウを閉じる
	if (ImGui::Button("close", { 70,30 }))
	{
		*current = -1;	// 選択状態を解除する
	}
}

//===========================================================================================================
// ブロックリスト
//===========================================================================================================
void EditWindowMain::BlockList()
{
	const char* BlockList[] = { "grass","stone" };
	int listbox_block_current = -1;

	// アイテムリスト
	ImGui::ListBox("BlockList", &listbox_block_current, BlockList, IM_ARRAYSIZE(BlockList), 4);
}

//===========================================================================================================
// ドロップしたファイルパスをセットする
//===========================================================================================================
void EditWindowMain::SetDropFilename(TCHAR* filename)
{
	const char* pfilename = "";

	// ドロップしたファイルパスから特定の文字を検索する
	pfilename = strstr(filename, SearchFilename);

	// 特定の文字列が存在しなかった場合
	if (pfilename == nullptr)
		return;

	// 特定の文字からのファイルパスをメンバー変数に代入する
	strcpy(&m_Filepass[0], pfilename);
}
