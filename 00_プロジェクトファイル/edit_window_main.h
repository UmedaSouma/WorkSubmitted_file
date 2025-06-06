#pragma once
//===========================================================================================================================================================
// 
// edit_window_main.cpp�̃w�b�_�[ [edit_window_main.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _EDIT_WINDOW_MAIN_H_
#define _EDIT_WINDOW_MAIN_H_
#include "main.h"

class EditWindowMain
{
public:

	// �E�B���h�E�̓���
	typedef enum
	{
		W_ACT_NONE = -1,
		W_ACT_SAVE,			// �Z�[�u
		W_ACT_LOAD,			// ���[�h
		W_ACT_OPEN_BLOCK,	// �u���b�N���X�g���J��
		W_ACT_MAX
	}WindowAction;

	EditWindowMain();
	~EditWindowMain();
	WindowAction DrawWindow();
	void ChangeWindowName(const char*str[128],int current);
	void WindowContents(int* current);
	void BlockList();
	void SetDropFilename(TCHAR* filename);	// �h���b�v�����t�@�C���p�X���Z�b�g����
	char* GetDropFilename() { return &m_Filepass[0]; }

private:
	// �E�B���h�E�^�C�v��
	typedef enum
	{
		WINDOWTYPE_NULL=-1,
		WINDOWTYPE_BLOCKLLIST,
		WINDOWTYPE_MAX
	}window_type;

	char m_Filepass[128];	// �t�@�C���p�X������
	const char* SearchFilename = "data\\";	// �������镶����
};

#endif // !EDIT_WINDOW_MAIN_H_