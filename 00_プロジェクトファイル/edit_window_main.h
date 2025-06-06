#pragma once
//===========================================================================================================================================================
// 
// edit_window_main.cppのヘッダー [edit_window_main.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _EDIT_WINDOW_MAIN_H_
#define _EDIT_WINDOW_MAIN_H_
#include "main.h"

class EditWindowMain
{
public:

	// ウィンドウの動作
	typedef enum
	{
		W_ACT_NONE = -1,
		W_ACT_SAVE,			// セーブ
		W_ACT_LOAD,			// ロード
		W_ACT_OPEN_BLOCK,	// ブロックリストを開く
		W_ACT_MAX
	}WindowAction;

	EditWindowMain();
	~EditWindowMain();
	WindowAction DrawWindow();
	void ChangeWindowName(const char*str[128],int current);
	void WindowContents(int* current);
	void BlockList();
	void SetDropFilename(TCHAR* filename);	// ドロップしたファイルパスをセットする
	char* GetDropFilename() { return &m_Filepass[0]; }

private:
	// ウィンドウタイプ列挙
	typedef enum
	{
		WINDOWTYPE_NULL=-1,
		WINDOWTYPE_BLOCKLLIST,
		WINDOWTYPE_MAX
	}window_type;

	char m_Filepass[128];	// ファイルパス文字列
	const char* SearchFilename = "data\\";	// 検索する文字列
};

#endif // !EDIT_WINDOW_MAIN_H_