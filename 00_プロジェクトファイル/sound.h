//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : ���{�S��
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�ꗗ
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_TITLE_BGM = 0,
	SOUND_LABEL_GAME_BGM,
	SOUND_LABEL_SWORD_SE,
	SOUND_LABEL_ITEMGET_SE,
	SOUND_LABEL_SLIMEJUMP_SE,
	SOUND_LABEL_SLIMETHROW_SE,
	SOUND_LABEL_SHOT_SE,

	//==================================================
	// �n�b�J�\���p
	//SOUND_TITLE_BGM,	// �^�C�g��bgm
	//SOUND_GAME_BGM,		// �Q�[��bgm
	//SOUND_RESULT_BGM,	// ���U���gbgm
	//SOUND_DEAD_SE,		// ���񂾂Ƃ��̉�
	//SOUND_PRESENT_SE,	// �͂����Ƃ��̉�
	//SOUND_WALK_SE,		// �����Ƃ��̉�


	SOUND_LABEL_MAX
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
