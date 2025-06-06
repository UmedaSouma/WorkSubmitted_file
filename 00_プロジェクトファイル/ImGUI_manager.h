#pragma once
//===========================================================================================================================================================
// 
// ImGUI_manager.cppのヘッダー [ImGUI_manager.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _IMGUI_MANAGER_H_
#define _IMGUI_MANAGER_H_
#include "main.h"

//===========================================================================================================
// インクルード
//===========================================================================================================
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <tchar.h>

#include "edit_window_main.h"
#include "edit_window_baseobj.h"
//===========================================================================================================
// クラス
//===========================================================================================================
class ImguiManager
{
public:
	ImguiManager();
	~ImguiManager();
	void Init();
	void SetUpContext();
	void Uninit();
	void TestWindow();
private:
	static EditWindowMain* m_pEditWindowMain;
	static EditWindowBaseobj* m_pEditWindowBaseobj;
public:
	static EditWindowMain* GetEditMain() { return m_pEditWindowMain; }
	static EditWindowBaseobj* GetEditBase() { return m_pEditWindowBaseobj; }
};




#endif // !IMGUI_MANAGER_H_