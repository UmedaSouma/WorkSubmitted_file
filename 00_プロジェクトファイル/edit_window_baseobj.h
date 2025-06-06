#pragma once
//===========================================================================================================================================================
// 
// edit_eindow_baseobj.cppのヘッダー [edit_eindow_baseobj.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _EDIT_WINDOW_BASEOBJ_H_
#define _EDIT_WINDOW_BASEOBJ_H_
#include "main.h"
#include "model.h"

class EditWindowBaseobj
{
public:
	EditWindowBaseobj();
	~EditWindowBaseobj();
	void Begin();	// 始動
	void End();		// 終了
	int SelectType(int& type);		// オブジェクトタイプ選択
	int MoveBar(D3DXVECTOR3& move);	// オブジェクトを動かすバー
	void PosNum(D3DXVECTOR3& pos);	// 位置を直接入れるブロック
	void ScaleNum(D3DXVECTOR3& scale);	// 拡大率を入れるブロック
	void RotNum(D3DXVECTOR3& rot);	// 角度を入れるブロック

	void DrawAddModelFileWindow();
private:
	const int m_MaxMoveSlider = 100;
	int m_MoveNum;
	D3DXVECTOR3 m_move;
	bool m_IsOpenAddtypeWindow;
	char m_Filepass[128];
	std::string m_AddResult = "none";	// 
};
#endif // !EDIT_WINDOW_BASEOBJ_H_