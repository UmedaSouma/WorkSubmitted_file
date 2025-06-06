#pragma once
//===========================================================================================================================================================
// 
// edit_eindow_baseobj.cpp�̃w�b�_�[ [edit_eindow_baseobj.h]
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
	void Begin();	// �n��
	void End();		// �I��
	int SelectType(int& type);		// �I�u�W�F�N�g�^�C�v�I��
	int MoveBar(D3DXVECTOR3& move);	// �I�u�W�F�N�g�𓮂����o�[
	void PosNum(D3DXVECTOR3& pos);	// �ʒu�𒼐ړ����u���b�N
	void ScaleNum(D3DXVECTOR3& scale);	// �g�嗦������u���b�N
	void RotNum(D3DXVECTOR3& rot);	// �p�x������u���b�N

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