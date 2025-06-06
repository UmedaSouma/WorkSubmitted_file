//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 高宮祐翔
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
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
	// ハッカソン用
	//SOUND_TITLE_BGM,	// タイトルbgm
	//SOUND_GAME_BGM,		// ゲームbgm
	//SOUND_RESULT_BGM,	// リザルトbgm
	//SOUND_DEAD_SE,		// 死んだときの音
	//SOUND_PRESENT_SE,	// 届けたときの音
	//SOUND_WALK_SE,		// 歩くときの音


	SOUND_LABEL_MAX
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
